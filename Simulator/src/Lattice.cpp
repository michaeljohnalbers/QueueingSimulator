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

// Terrible design, but its small enough to let slide.
int32_t glbIndividualsLeft = 0;

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

  const int MAX_DESIRED_LEVELS = 5;
  int maxLevels = omp_get_max_active_levels();
  if (maxLevels < MAX_DESIRED_LEVELS)
  {
    omp_set_max_active_levels(5);
    Logger::log("  Setting Maximum Nesting Depth to: " +
                std::to_string(MAX_DESIRED_LEVELS));
  }

  Logger::log("  Nesting Depth: " +
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
                                     myNumberIndividuals, myRunConfiguration);
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
      Individual testIndividual(mass, 0, 0, 0, position, 0, myRunConfiguration);

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
                                       orientation, position, ii+1,
                                       myRunConfiguration);

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
  double totalTime = runTime.count();

  Logger::log("");
  Logger::log("========== Simulation Report ==========");
  Logger::log("Run time: " + std::to_string(totalTime) + "s");
  Logger::log("Number Frames: " + std::to_string(myTotalNumberFrames));
  Logger::log("Average Frame Time: " +
              std::to_string(totalTime / myTotalNumberFrames) + "s");
  Logger::log("");
  Logger::log("========== Individual Data ==========");

  typedef struct
  {
    float averageDelta = 0.0;
    int32_t count = 0;
    void average()
    {
      if (count != 0)
      {
        averageDelta /= count;
      }
    }
    std::string print()
    {
      std::string data =
        "        Count: " + std::to_string(count) + "\n"
        "Average Delta: " + std::to_string(averageDelta) + "s";
      return data;
    }

  } ErrorData;
  ErrorData overIdeal, underIdeal;

  float maxActualTime = 0.0;
  float maxErrorDelta = 0.0;
  float minActualTime = FLT_MAX;
  float minErrorDelta = FLT_MAX;

  for (int32_t ii = 0; ii < myNumberIndividuals; ++ii)
  {
    auto individual = myIndividuals[ii];
    runTime = individual->getExitTime() - mySimulationStartTime;
    float actualTime = runTime.count();

    if (QS::Benchmark == myRunConfiguration)
    {
      if (actualTime > maxActualTime)
      {
        maxActualTime = actualTime;
      }

      if (actualTime < minActualTime)
      {
        minActualTime = actualTime;
      }


      float idealTime = individual->getIdealStraightLineTime();
      float delta = actualTime - idealTime;
      ErrorData *data = &overIdeal;
      if (delta < 0)
      {
        data = &underIdeal;
      }

      if (delta > maxErrorDelta)
      {
        maxErrorDelta = delta;
      }

      if (delta < minErrorDelta)
      {
        minErrorDelta = delta;
      }

      data->averageDelta += delta;
      data->count++;

      Logger::log("Individual " + std::to_string(ii+1) + ", Max Speed: " +
                  std::to_string(individual->getMaximumSpeed()) +
                  "m/s, Original Position: " +
                  EigenHelper::print(individual->getOriginalPosition()));
      Logger::log("          Time in Simulation: " +
                  std::to_string(actualTime) + "s");
      Logger::log("     Total Distance Traveled: " +
                  std::to_string(individual->getDistanceTraveled()) + "m");
      Logger::log("    Ideal Straight Line Time: " +
                  std::to_string(idealTime) + "s");
      Logger::log("      Delta (Actual - Ideal): " +
                  std::to_string(delta) + "s");
    }
    else
    {
       Logger::log("Individual " + std::to_string(ii+1));
       Logger::log("     Time in Simulation: " +
                   std::to_string(actualTime) + "s");
       Logger::log("Total Distance Traveled: " +
                   std::to_string(individual->getDistanceTraveled()) + "m");
    }
  }

  if (QS::Benchmark == myRunConfiguration)
  {
    underIdeal.average();
    overIdeal.average();
    Logger::log("========== Group Data ==========");
    Logger::log("Minimum Actual Time: " + std::to_string(minActualTime) + "s");
    Logger::log("Maximum Actual Time: " + std::to_string(maxActualTime) + "s");
    Logger::log("Minimum Error Delta: " + std::to_string(minErrorDelta) + "s");
    Logger::log("Maximum Error Delta: " + std::to_string(maxErrorDelta) + "s");
    Logger::log("Over Ideal Time\n" + overIdeal.print());
    Logger::log("Under Ideal Time\n" + underIdeal.print());
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
  std::string benchmarkOrSim;
  configFile >> benchmarkOrSim;
  std::transform(benchmarkOrSim.begin(), benchmarkOrSim.end(),
                 benchmarkOrSim.begin(), ::tolower);

  if ("benchmark" == benchmarkOrSim)
  {
    myRunConfiguration = QS::Benchmark;
  }
  else if ("simulation" == benchmarkOrSim)
  {
    myRunConfiguration = QS::Simulation;
  }
  else
  {
    throw std::logic_error("Invalid benchmark/simulation option given, \"" +
                           benchmarkOrSim + "\". Must be either " +
                           "\"benchmark\" or \"simulation\".");
  }

  Logger::log("Simulation Configuration Data");
  Logger::log("  Run Configuration: " + benchmarkOrSim);
  Logger::log("  Number of Individuals: " +
              std::to_string(myNumberIndividuals));
  Logger::log("  World Size (length x height): " + std::to_string(myLength)
              + "m x " + std::to_string(myHeight) + "m");
  Logger::log("  Bucket Configuration: " + myBucketConfiguration);
  Logger::log("  Neighbor Search Radius (m): " + std::to_string(searchRadius));
  Logger::log("  N (# neighbors): " + std::to_string(n));
  Logger::log("  Random Number Engine Seed: " +
              std::to_string(randomNumberEngineSeed));

  glbIndividualsLeft = myNumberIndividuals;
  Exit::setRunConfiguration(myRunConfiguration);
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
