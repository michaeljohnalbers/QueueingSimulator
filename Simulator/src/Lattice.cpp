/**
 * @file Lattice.cpp
 * @brief Implementation of the Lattice class.
 *
 * @author Michael Albers
 */

#include <cctype>
#include <cfloat>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <thread>

#include <omp.h>

#include <Bucket.h>
#include <EigenHelper.h>
#include <Exit.h>
#include <Individual.h>
#include <Lattice.h>
#include <Logger.h>
#include <NearestN.h>

//*****************
// Lattice::Lattice
//*****************
Lattice::Lattice(const std::string &theConfigFileName) :
  myConfigFileName(theConfigFileName)
{
}

//******************
// Lattice::~Lattice
//******************
Lattice::~Lattice()
{
  if (myBuckets != nullptr)
  {
    for (auto ii = 0; ii < myNumberBuckets; ++ii)
    {
      delete myBuckets[ii];
    }
    delete [] myBuckets;
  }

  if (myBucketLattice != nullptr)
  {
    delete [] myBucketLattice[0];
    delete [] myBucketLattice;
  }

  if (myIndividuals != nullptr)
  {
    for (auto ii = 0; ii < myNumberIndividuals; ++ii)
    {
      delete myIndividuals[ii];
    }
    delete [] myIndividuals;
  }
}
//*************************
// Lattice::configureOpenMP
//*************************
void Lattice::configureOpenMP()
{
  Logger::log("OpenMP Configuration");

#ifndef SERIAL
  int openMPMaxThreads = omp_get_max_threads();
  Logger::log("  Maximum Size of Thread Team: " +
              std::to_string(openMPMaxThreads));

  Logger::log("  Process Thread Limit: " +
              std::to_string(omp_get_thread_limit()));

  // If nesting is disabled the omp_get_level will still return the number of
  // parallel regions around the omp_get_level call. Use the
  // omp_get_active_level to see what the actual nesting level is.
  bool nestedParallelismEnabled = omp_get_nested();
  if (! nestedParallelismEnabled)
  {
    omp_set_nested(true);
  }

  Logger::log("  Nested Parallelism Enabled: " +
              std::string(omp_get_nested() ? "true" : "false"));

  Logger::log("  Maximum Nesting Depth: " +
              std::to_string(omp_get_max_active_levels()));
#else
  Logger::log("  Compile-time -DSERIAL defined, no parallelism.");
#endif
}

//***********************
// Lattice::createBuckets
//***********************
void Lattice::createBuckets() throw (std::exception)
{
  /* World
    --------------- ^
    |             | |
    |             | h
    |             | e
    |             | i
    |             | g
    |             | h
    |             | t
    |             | |
    |             | v
    ---------------
    <--  length -->
   */

  auto widthRemainder = myHeight % myLatticeRows;
  if (widthRemainder != 0)
  {
    myHeight = myHeight - widthRemainder;
    Logger::log("Adjusted world width to accommodate number of rows. "
                "New width: " + std::to_string(myHeight));
  }

  auto bucketHeight = myHeight / myLatticeRows;

  auto lengthRemainder = myLength % myLatticeColumns;
  if (lengthRemainder != 0)
  {
    myLength = myLength - lengthRemainder;
    Logger::log("Adjusted world length to accommodate number of columns. "
                "New length: " + std::to_string(myLength));
  }

  Eigen::Vector2f exitPosition(0.0, myHeight / 2.0);
  Exit::setPosition(exitPosition);
  Logger::log("Placing exit at " + EigenHelper::print(Exit::getPosition()) +
              ", radius: " + std::to_string(Exit::getRadius()) + ".");

  auto bucketLength = myLength / myLatticeColumns;

  myBuckets = new Bucket* [myNumberBuckets];
  myBucketLattice = new Bucket**[myLatticeRows];
  myBucketLattice[0] = new Bucket*[myLatticeRows * myLatticeColumns];
  for (int32_t ii = 1; ii < myLatticeRows; ++ii)
  {
    myBucketLattice[ii] = myBucketLattice[ii - 1] + myLatticeColumns;
  }

  for (auto row = 0, ii = 0; row < myLatticeRows; ++row)
  {
    for (auto col = 0; col < myLatticeColumns; ++col)
    {
      Eigen::Vector2f origin(col * bucketLength, row * bucketHeight);
      Bucket *newBucket = new Bucket(origin, bucketHeight, bucketLength,
                                     myNumberIndividuals);
      myBucketLattice[row][col] = newBucket;
      myBuckets[ii++] = newBucket;
    }
  }

  for (auto row = 0; row < myLatticeRows; ++row)
  {
    for (auto col = 0; col < myLatticeColumns; ++col)
    {
      for (auto rowOffset = -1; rowOffset <= 1; ++rowOffset)
      {
        for (auto colOffset = -1; colOffset <= 1; ++colOffset)
        {
          auto adjacentRow = row + rowOffset;
          auto adjacentCol = col + colOffset;
          if ((adjacentRow >= 0 && adjacentRow < myLatticeRows) &&
              (adjacentCol >= 0 && adjacentCol < myLatticeColumns) &&
              (! (rowOffset == 0 && colOffset == 0)))
          {
            myBucketLattice[row][col]->addAdjacentBucket(
              myBucketLattice[adjacentRow][adjacentCol], rowOffset, colOffset);
          }
        }
      }
    }
  }
}

//***************************
// Lattice::createIndividuals
//***************************
void Lattice::createIndividuals()
{
  auto startTime = std::chrono::system_clock::now();

  myIndividuals = new Individual* [myNumberIndividuals];
  std::uniform_real_distribution<float> xCoordinateGen(0, myLength);
  std::uniform_real_distribution<float> yCoordinateGen(0, myHeight);
  // Approximate walking speeds, m/s.
  std::uniform_real_distribution<float> maxSpeedGen(2.0, 5.0);
  // 100 - 200 lbs (converted to kg).
  std::uniform_real_distribution<float> massGen(45.352, 90.718);
  std::uniform_real_distribution<float> orientationGen(0, 1.0);

  // Rough approximation of force generator. Scaled down from what Usain Bolt
  // does (surprisingly, this was the best (simple) information I could find
  // online). In Newtons. Refer to:
  // http://www.gizmag.com/usain-bolt-fastest-man-physics-analysis/28457/
  // TODO: using this was catastophic, need to make it work though
  std::uniform_real_distribution<float> forceGen(600.0, 750.0);

  // Keep this serial to maintain consistency with random number generators
  // across runs of the simulator.
  for (auto ii = 0; ii < myNumberIndividuals; ++ii)
  {
    float maximumSpeed = maxSpeedGen(myRandomEngine);
    float mass = massGen(myRandomEngine);
    float orientation = orientationGen(myRandomEngine);
    float maximumAcceleration = 10.0;

    bool goodPosition = false;
    Eigen::Vector2f position;
    while (! goodPosition)
    {
      float x = xCoordinateGen(myRandomEngine);
      float y = yCoordinateGen(myRandomEngine);
      position[0] = x;
      position[1] = y;
      Individual testIndividual(mass, 0, 0, 0, position, 0);

      goodPosition = true;
#ifndef SERIAL
#     pragma omp parallel for shared(goodPosition)
#endif
      for (auto jj = 0; jj < ii; ++jj)
      {
        if (myIndividuals[jj]->collision(&testIndividual))
        {
          goodPosition = false;
        }
      }
    }

    myIndividuals[ii] = new Individual(mass, maximumSpeed, maximumAcceleration,
                                       orientation, position, ii+1);

    float distance = EigenHelper::distance(position, Exit::getPosition());
    Logger::log("Generated person " + std::to_string(ii+1) + " " +
                 + " at position " + EigenHelper::print(position) +
                " (distance: " + std::to_string(distance) + ").\n" +
                myIndividuals[ii]->getBasicData());

#ifndef SERIAL
#   pragma omp parallel for
#endif
    for (auto bucket = 0; bucket < myNumberBuckets; ++bucket)
    {
      if (myBuckets[bucket]->containsPoint(myIndividuals[ii]->getPosition()))
      {
        myBuckets[bucket]->receiveIndividual(myIndividuals[ii]);
      }
    }
  }

  auto stopTime = std::chrono::system_clock::now();

  std::chrono::duration<double> duration = stopTime - startTime;
  Logger::log("Created Individuals in: " + std::to_string(duration.count()) +
              "s.");
}

//******************************
// Lattice::determineLatticeGrid
//******************************
void Lattice::determineLatticeGrid() throw (std::exception)
{
  std::regex numberRegex("[0-9]+", std::regex::basic);
  std::regex multipleRegex("x[0-9]+", std::regex::extended);

  auto concurrency = omp_get_max_threads();

  if ("auto" == myBucketConfiguration)
  {
    myNumberBuckets = concurrency;
  }
  else if (std::regex_search(myBucketConfiguration, multipleRegex))
  {
    // TODO: g++ < 4.9 doesn't handle regex implementation very well.
    // in this case it throws an error saying "x10" isn't valid.
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53631
    myNumberBuckets =
      std::stoi(myBucketConfiguration.substr(1, std::string::npos)) *
      concurrency;
  }
  else //if (std::regex_match(myBucketConfiguration, numberRegex))
  {
    myNumberBuckets = std::stoi(myBucketConfiguration);
  }
#if 0 // TODO: remove when using >= g++ 4.9
  else
  {
    throw std::domain_error("Invalid option for bucket configuration: \"" +
                            myBucketConfiguration + "\"");
  }
#endif

  if (0 == myNumberBuckets)
  {
    throw std::logic_error(
      "Unable to determine the number of buckets to use. "
      "(Hardware concurrency: " + std::to_string(concurrency));
  }

  // Right now it's far simpler to only allow certain numbers of rows. Perhaps
  // in the future this could be optimized to find the correct number of rows
  // and columns given the world size and number of buckets.
  if (myNumberBuckets % 3 == 0)
  {
    myLatticeRows = 3;
  }
  else if (myNumberBuckets % 2 == 0)
  {
    myLatticeRows = 2;
  }
  else if (1 == myNumberBuckets)
  {
    myLatticeRows = 1;
  }
  else
  {
    throw std::domain_error("Invalid number of buckets, " +
                            std::to_string(myNumberBuckets) +
                            ", must be a multiple of 2 or 3.");
  }

  myLatticeColumns = myNumberBuckets / myLatticeRows;

  // This is to make sure the smaller dimension has the smaller number of
  // sub-divisions. All calculations up to this point have assumed a
  // wold shape like:
  // ----------
  // |  |  |  |  (1 row, 3 cols)
  // ----------
  // If the world is taller than longer swap the row & column values to match:
  // ----
  // |  |  (3 rows, 1 col)
  // ----
  // |  |
  // ----
  // |  |
  // ----
  // This probably isn't needed but was useful in conceptulizing the world.
  if (myHeight > myLength)
  {
    std::swap(myLatticeRows, myLatticeColumns);
  }

  Logger::log("Number of buckets in Lattice: " +
              std::to_string(myNumberBuckets));
  Logger::log("Buckets grid is [" + std::to_string(myLatticeRows) +
              "][" + std::to_string(myLatticeColumns) + "].");
}

//*********************
// Lattice::frameUpdate
//*********************
void Lattice::frameUpdate()
{
  auto frameStart = std::chrono::system_clock::now();

  // Weirdest code I've ever written. The simulation runs very poorly if the
  // frame time is too short. Individuals simply take too long to exit as
  // the time correction of the steering force shrinks the acceleration &
  // velocity vectors to infintesimal magnitude. This really only seems to
  // happen when using a serial build and very small numbers (i.e., 1)
  // of Individuals. The sleep time was determined empirically on my laptop.
  if (myFrameTime < 0.001)
  {
    static const std::chrono::microseconds sleepTime(10);
    std::this_thread::sleep_for(sleepTime);
  }

#ifndef SERIAL
# pragma omp parallel for
#endif
  for (auto ii = 0; ii < myNumberBuckets; ++ii)
  {
    myBuckets[ii]->frameUpdate(myFrameTime);
  }

  // This must be done serially. Underlying functions are not thread-safe.
  for (auto ii = 0; ii < myNumberBuckets; ++ii)
  {
    myBuckets[ii]->postFrameUpdateAdjustments();
  }

  auto frameDuration = std::chrono::system_clock::now() - frameStart;

  float period = static_cast<float>(std::chrono::system_clock::period::num) /
    static_cast<float>(std::chrono::system_clock::period::den);

  myFrameTime = static_cast<float>(frameDuration.count()) * period;

  ++myTotalNumberFrames;
}

//************************
// Lattice::generateReport
//************************
void Lattice::generateReport()
{
  std::chrono::duration<double> runTime =
    mySimulationStopTime - mySimulationStartTime;
  float totalTime = runTime.count();

  Logger::log("=======================================");
  Logger::log("           Simulation Report           ");
  Logger::log("=======================================");
  Logger::log("");

  Logger::log("---------- General Data ----------");
  Logger::log("          Run Time: " + std::to_string(totalTime) + "s");
  Logger::log("     Number Frames: " + std::to_string(myTotalNumberFrames));
  Logger::log("Average Frame Time: " +
              std::to_string(totalTime / myTotalNumberFrames) + "s");
  Logger::log("     Frames/Second: " +
              std::to_string(myTotalNumberFrames / totalTime));
  Logger::log("");

  Logger::log("---------- Individual Data ----------");
  for (int32_t ii = 0; ii < myNumberIndividuals; ++ii)
  {
    auto individual = myIndividuals[ii];

    runTime = individual->getExitTime() - mySimulationStartTime;
    float timeInSimulation = runTime.count();

    Logger::log("Individual " + std::to_string(ii+1));
    Logger::log("     Time in Simulation: " +
                std::to_string(timeInSimulation) + "s");
    Logger::log("Total Distance Traveled: " +
                std::to_string(individual->getDistanceTraveled()) + "m");
  }

  Logger::log("========== End Report ==========");
}

//************************
// Lattice::loadConfigFile
//************************
void Lattice::loadConfigFile() throw (std::exception)
{
  std::ifstream configFile;
  configFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  configFile.open(myConfigFileName);
  configFile >> myNumberIndividuals;
  configFile >> myLength >> myHeight;
  configFile >> myBucketConfiguration;
  float searchRadius;
  configFile >> searchRadius;
  int32_t n;
  configFile >> n;
  int32_t randomNumberEngineSeed;
  configFile >> randomNumberEngineSeed;

  Logger::log("Simulation Configuration Data");
  Logger::log("  Number of Individuals: " +
              std::to_string(myNumberIndividuals));
  Logger::log("  World Size (length x height): " + std::to_string(myLength)
              + "m x " + std::to_string(myHeight) + "m");
  Logger::log("  Bucket Configuration: " + myBucketConfiguration);
  Logger::log("  Neighbor Search Radius (m): " + std::to_string(searchRadius));
  Logger::log("  N (# neighbors): " + std::to_string(n));
  Logger::log("  Random Number Engine Seed: " +
              std::to_string(randomNumberEngineSeed));

  NearestN::setN(n);
  NearestN::setSearchRadius(searchRadius);
  myRandomEngine.seed(randomNumberEngineSeed);
}

//************************
// Lattice::runSimulation
//************************
void Lattice::runSimulation()
{
  try
  {
    Logger::log("Initializing simulation from configuration file: " +
                myConfigFileName);

    loadConfigFile();
    configureOpenMP();
    determineLatticeGrid();
    createBuckets();
    createIndividuals();

    Logger::log("Initialization complete.");

    Logger::log("Simulation starting.");

    mySimulationStartTime = std::chrono::system_clock::now();

    while (Exit::getRankToExit() <= myNumberIndividuals)
    {
      frameUpdate();
    }

    mySimulationStopTime = std::chrono::system_clock::now();

    Logger::log("Simulation finished.");

    generateReport();
  }
  catch (const std::exception &exception)
  {
    Logger::log(std::string("Error initializing simulation: ") +
                exception.what());
  }
}
