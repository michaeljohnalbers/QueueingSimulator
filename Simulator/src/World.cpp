/**
 * @file World.cpp
 * @brief World class
 * @author Michael Albers
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <World.h>

using namespace std::chrono;

World::World(int32_t theNumberActors,
             int32_t theNumberRows,
             int32_t theNumberColumns,
             int32_t theSeed,
             const std::string &theOutputFile) :
  myActorGenerator(theNumberActors, theNumberRows, theNumberColumns, theSeed),
  myExit(0,0),
  myOutputFileName(theOutputFile)
{
  // auto t = high_resolution_clock::now();
  // timespec sleep = {0, 123456789};
  // nanosleep(&sleep, nullptr);
  // auto u = high_resolution_clock::now();

  // std::cout << "Duration: " << duration_cast<milliseconds>(u-t).count() << "ms."
  //           << std::endl;

  // std::cout << "steady clock - min: " << high_resolution_clock::duration::min().count()
  //           << ", max: " << high_resolution_clock::duration::max().count()
  //           << ", " << (treat_as_floating_point<high_resolution_clock::duration>::value ? "FP" : "integral") << std::endl;
  // std::exit(0);

}

World::World(const std::string &theInputFile,
             const std::string &theOutputFile) :
  myActorGenerator(theInputFile),
  myExit(0,0),
  myOutputFileName(theOutputFile)
{
}

World::~World()
{
  if (nullptr != myWorld)
  {
    delete [] myWorld[0];
    delete [] myWorld;
  }
  for (Actor *a : myActors)
  {
    delete a;
  }
}

void World::calculateAndWriteActorStatistics()
  throw (std::ios_base::failure)
{
  std::vector<uint32_t> listOfNumberOfMoves;
  std::vector<std::time_t> timesInWorld;
  uint32_t totalMoves = 0;
  std::time_t totalTimeInWorld = 0;
  for (Actor *actor : myActors)
  {
    uint32_t numberOfMoves = actor->getPreviousPositions().size();
    listOfNumberOfMoves.push_back(numberOfMoves);
    totalMoves += numberOfMoves;

    std::time_t actorTimeInWorld = actor->getEndTime() - myStartTime;
    timesInWorld.push_back(actorTimeInWorld);
    totalTimeInWorld += actorTimeInWorld;
  }

  float numActorsAsFloat = static_cast<float>(myActors.size());
  float averageMoves =
    static_cast<float>(totalMoves) / numActorsAsFloat;
  float averageTime =
    static_cast<float>(totalTimeInWorld) / numActorsAsFloat;

  float medianMoves = median(listOfNumberOfMoves);
  float medianTime = median(timesInWorld);

  myOutputFile << "Actor Statistics" << std::endl
               << "----------------" << std::endl
               << "  Average time in world: " << averageTime << std::endl
               << "   Median time in world: " << medianTime << std::endl
               << "Average Number of Moves: " << averageMoves << std::endl
               << " Median Number of Moves: " << medianMoves << std::endl
               << std::endl;
}

void World::createOutputFile() throw (std::logic_error)
{
  try
  {
    std::cout << "Opening simulation results file \""
              << myOutputFileName << "\"." << std::endl;
    myOutputFile.exceptions(std::ios::badbit | std::ios::failbit |
                            std::ios::eofbit);
    myOutputFile.open(myOutputFileName, std::ios::out);
  }
  catch (std::ios_base::failure &exception)
  {
    std::ostringstream error;
    error << "Failed to open simulation results file \"" << myOutputFileName
          << "\": " << exception.what();
    throw std::logic_error(error.str());
  }
}

void World::createSimulationReport() throw (std::logic_error)
{
  try
  {
    static const std::string BIG_DELIM(
      "================================================");

    myOutputFile << BIG_DELIM << std::endl
                 << "   Simulation Results" << std::endl
                 << BIG_DELIM << std::endl << std::endl
                 << "Simulation Configuration Data" << std::endl
                 << "-----------------------------" << std::endl
                 << "         Configuration: Serial" << std::endl
                 << "      Number of Actors: " << myActors.size() << std::endl
                 << "World Size (rowsxcols): " << myNumberRows << "x"
                 << myNumberColumns << std::endl
                 << "         Exit Position: "
                 << std::get<Actor::ROW>(myExit) << "x"
                 << std::get<Actor::COL>(myExit) << std::endl
                 << std::endl
                 << "Simulation Run Data" << std::endl
                 << "-------------------" << std::endl
      // TODO: print times in nice format as well.
                 << "       Start Time: " << myStartTime << std::endl
                 << "         End Time: " << myEndTime << std::endl
                 << "       Total Time: " << (myEndTime - myStartTime)
                 << std::endl
                 << " Number of Rounds: " << myNumberRounds << std::endl
                 << std::endl
                 << "Actor Data" << std::endl
                 << "----------" << std::endl;
    for (Actor *actor : myActors)
    {
      myOutputFile << "Rank: "
                   << std::setw(std::log10(myActors.size())) << actor->getRank()
                   << ", Exit Time: " << actor->getEndTime()
                   << ", Time In World: " << (actor->getEndTime() - myStartTime)
                   << ", Number of Moves: " << actor->getPreviousPositions().size()
                   << std::endl;
    }

    myOutputFile << std::endl;

    calculateAndWriteActorStatistics();
  }
  catch (std::ios_base::failure &exception)
  {
    std::ostringstream error;
    error << "Failed to write simulation results data to results file \""
          << myOutputFileName << "\": " << exception.what()
          << std::endl;
    throw std::logic_error(error.str());
  }
}

void World::createWorld() throw (std::logic_error)
{
  myNumberRows = myActorGenerator.getNumberRows();
  myNumberColumns = myActorGenerator.getNumberColumns();

  // TODO: for later, try arbitrary exit position (i.e., in middle of world)
  myExit = Actor::Position(myNumberRows/2,0);

  std::cout << "Creating " << myNumberRows << " x " << myNumberColumns
            << " world with exit at world["
            << std::get<Actor::ROW>(myExit) << "]["
            << std::get<Actor::COL>(myExit) << "]."
            << std::endl;

  myWorld = new Actor**[myNumberRows];
  myWorld[0] = new Actor*[myNumberRows * myNumberColumns];
  for (int32_t ii = 1; ii < myNumberColumns; ++ii)
  {
    myWorld[ii] = myWorld[ii-1] + myNumberColumns;
  }

  for (int32_t row = 0; row < myNumberRows; ++row)
  {
    for (int32_t column = 0; column < myNumberColumns; ++column)
    {
      myWorld[row][column] = nullptr;
    }
  }
}

void World::initializeSimulation() throw (std::logic_error)
{
  std::cout << "Initializing simulation." << std::endl;
  myActorGenerator.generateActors();
  createWorld();
  placeActors();
  createOutputFile();
}

template<class T>
float World::median(std::vector<T> theList)
{
  float median = 0.0;
  uint32_t middleElement = theList.size() / 2;
  std::sort(theList.begin(), theList.end());
  if (theList.size() % 2 == 1)
  {
    median = theList[middleElement];
  }
  else
  {
    median = 0.5*(static_cast<float>(
                    theList[middleElement] +
                    theList[middleElement-1]));
  }
  return median;
}

void World::moveIntoLine() throw (std::logic_error)
{
  int32_t exitRow = std::get<Actor::ROW>(myExit);
  int32_t exitCol = std::get<Actor::COL>(myExit);

  while (static_cast<uint32_t>(myNextRankToExit) <= myActors.size())
  {
    if (myNumberRounds % myActorsInWorld.size() == 0)
    {
      std::cout << "Beginning of round " << myNumberRounds
                << ". Next Rank that can exit == " << myNextRankToExit
                << "." << std::endl;
    }

    for (auto actorIter = myActorsInWorld.begin();
         actorIter != myActorsInWorld.end(); /* No increment needed */)
    {
      Actor *actor = *actorIter;
      int32_t actorRow = std::get<Actor::ROW>(actor->getCurrentPosition());
      int32_t actorCol = std::get<Actor::COL>(actor->getCurrentPosition());

      // Assumes the exit is in the far left column. Since each actor has
      // unique rank, the desired column position will be unique across actors.
      int32_t desiredRow = exitRow;
      int32_t desiredColumn = actor->getRank() - myNextRankToExit;

      int32_t rowToMoveTo = desiredRow;
      int32_t colToMoveTo = desiredColumn;

      int32_t backOffRowDirection = 1;
      int32_t backOffColDirection = 1;

      if (actorRow < desiredRow)
      {
        rowToMoveTo = actorRow + 1;
        backOffRowDirection = -1;
      }
      else if (actorRow > desiredRow)
      {
        rowToMoveTo = actorRow - 1;
      }

      if (actorCol > desiredColumn)
      {
        colToMoveTo = actorCol - 1;
      }

      // The order of the ifs and else-ifs below (and in the first else block)
      // are hugely critical. The actor tries to move to its desired column
      // first, but backs off by row first. This helps prevent the deadlock
      // which would otherwise occur when the actor is in the line, but in the
      // wrong position in line. In that case, without the ordering below, the
      // actor just backs up a column, then in the next round moves forward one
      // column instead of moving around the line.
      //
      // Without ordering:
      //  1: [exit] [rank2] [rank3] [rank1]
      //  2: [exit] [rank2] [rank3] [null] [rank1]
      //  3: [exit] [rank2] [ranke] [rank1]
      //
      // With ordering:
      //  1: [exit] [rank2] [rank3] [rank1]
      //  2:                        [rank1]
      //     [exit] [rank2] [rank3]
      //  3:                [rank1]
      //     [exit] [rank2] [rank3]
      //  4: etc. rank1 moves down the line

      bool willMove = true;
      if (nullptr == myWorld[rowToMoveTo][colToMoveTo])
      {
        // TODO: move actor
      }
      else if (nullptr == myWorld[actorRow][colToMoveTo])
      {
        // Optimal position and just row-better positions weren't open. Try
        // just moving one column closer.
        rowToMoveTo = actorRow;
      }
      else if (nullptr == myWorld[rowToMoveTo][actorCol])
      {
        // Couldn't move to optimal postion. See if you can get a row closer.
        colToMoveTo = actorCol;
      }
      else
      {
        int32_t backOffRow = actorRow + backOffRowDirection * 1;
        int32_t backOffCol = actorCol + backOffColDirection * 1;

        if (nullptr == myWorld[backOffRow][actorCol])
        {
          rowToMoveTo = backOffRow;
          colToMoveTo = actorCol;
        }
        else if (nullptr == myWorld[actorRow][backOffCol])
        {
          rowToMoveTo = actorRow;
          colToMoveTo = backOffCol;
        }
        else if (nullptr == myWorld[backOffRow][backOffCol])
        {
          rowToMoveTo = backOffRow;
          colToMoveTo = backOffCol;
        }
        else
        {
          willMove = false;
          // TODO: need to try closer row but further col & vice versa
        }
      }

      if (willMove)
      {
        // TODO: run-time debug level (cmd line option)
#ifdef DEBUG
        std::cout << "Moving actor with rank " << actor->getRank()
                  << " from world[" << actorRow << "][" << actorCol
                  << "] to position world[" << rowToMoveTo << "]["
                  << colToMoveTo << "]. Desired position is world["
                  << desiredRow << "][" << desiredColumn << "]. "
                  << "Exit is world[" << exitRow << "][" << exitCol << "]."
                  << std::endl;
#endif
        std::swap(myWorld[rowToMoveTo][colToMoveTo],
                  myWorld[actorRow][actorCol]);
        actor->setCurrentPosition(Actor::Position(rowToMoveTo, colToMoveTo));
      }

      if (rowToMoveTo == exitRow && colToMoveTo == exitCol)
      {
        myNextRankToExit++;
        myWorld[exitRow][exitCol] = nullptr;
        actor->setEndTime();
        actorIter = myActorsInWorld.erase(actorIter);
        std::cout << "Actor with rank " << actor->getRank() << " exited."
                  << std::endl;
      }
      else
      {
        ++actorIter;
      }
    }

    myNumberRounds++;
  }
}

void World::placeActors() throw (std::logic_error)
{
  std::cout << "Placing " << myActorGenerator.getActors().size()
            << " actors in world." << std::endl;

  const std::vector<Actor> &actors = myActorGenerator.getActors();
  for (uint32_t ii = 0; ii < actors.size(); ++ii)
  {
    Actor *newActor = new Actor(actors[ii]);

    Actor::Position actorPosition = newActor->getCurrentPosition();
    int32_t row = std::get<Actor::ROW>(actorPosition);
    int32_t col = std::get<Actor::COL>(actorPosition);
#ifdef DEBUG // TODO: run-time debug
    std::cout << "Placing actor with rank " << newActor->getRank()
              << " at world[" << row << "][" << col << "]." << std::endl;
#endif
    myWorld[row][col] = newActor;
    myActors.push_back(newActor);
    myActorsInWorld.push_back(newActor);
  }
}

void World::runSimulation()
{
  // TODO: change time_t to C++ stuff?
  try
  {
    myStartTime = std::time(nullptr);

    initializeSimulation();

    // TODO: need more precision than time_t
    char buff[256];
    std::strftime(buff, sizeof buff, "%FT%T", std::localtime(&myStartTime));
    std::cout << "Simulation started at " << buff << std::endl;

    moveIntoLine();

    myEndTime = std::time(nullptr);
    std::strftime(buff, sizeof buff, "%FT%T", std::localtime(&myEndTime));
    std::cout << "Simulation ended at " << buff << std::endl;

    createSimulationReport();
  }
  catch (const std::logic_error &exception)
  {
    std::cerr << "Simulation failed: " << exception.what()
              << std::endl;
  }
}
