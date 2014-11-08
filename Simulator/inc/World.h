#ifndef _World_h_
#define _World_h_

/**
 * @file World.h
 * @brief Class which respresents the world containing the actors.
 *
 * @author Michael Albers
 */

#include <Actor.h>
#include <ActorGenerator.h>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <vector>

/**
 * TODO: fill in
 */
class World
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default Constructor.
   */
  World() = delete;

  /**
   * Copy constructor.
   * @param Object to copy.
   */
  World(const World&) = delete;

  /**
   * Constructor.
   * @param theNumberActors Number of actors in the world.
   * @param theNumberRows Number of rows in world grid.
   * @param theNumberColumns Number of columns in world grid.
   * @param theSeed Seed for generating actor positions.
   * @param theOutputFile File name to which simulation results to.
   */
  World(int32_t theNumberActors,
        int32_t theNumberRows,
        int32_t theNumberColumns,
        int32_t theSeed,
        const std::string &theOutputFile);

  /**
   * Constructor. Initializes the world from the data in the given file.
   * @param theInputFile - Input file.
   * @param theOutputFile File name to which simulation results to.
   */
  World(const std::string &theInputFile,
        const std::string &theOutputFile);

  /**
   * Destructor.
   */
  ~World();

  /**
   * Assignment operator.
   * @param Object to copy.
   * @return This object.
   */
  World& operator=(const World&) = delete;

  /**
   * Runs the simulation.
   */
  void runSimulation();

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  void calculateAndWriteActorStatistics()
    throw (std::ios_base::failure);

  void createOutputFile() throw (std::logic_error);

  void createSimulationReport() throw (std::logic_error);

  void createWorld() throw (std::logic_error);

  void initializeSimulation() throw (std::logic_error);

  template<class T>
  static float median(std::vector<T> theList);

  void moveIntoLine() throw (std::logic_error);

  void placeActors() throw (std::logic_error);

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** All of the actors in the world. */
  // TODO: change to smart pointers
  std::vector<Actor*> myActors;

  /** All actors still in the world (i.e., which ones haven't exited yet.) */
  std::vector<Actor*> myActorsInWorld;

  ActorGenerator myActorGenerator;

  /** Location of the world exit. */
  Actor::Position myExit;

  /** Rank of the next actor that can exit the world. */
  int32_t myNextRankToExit = 1;

  /** Number of columns in the world. */
  int32_t myNumberColumns = 0;

  /** Number of rounds of the simulation. */
  uint32_t myNumberRounds = 1;

  /** Number of rows in the world. */
  int32_t myNumberRows = 0;

  /** File with simulation results. */
  std::ofstream myOutputFile;

  /** File name to which simulation results to. */
  std::string myOutputFileName;

  /** Simulation start time. */
  std::time_t myStartTime = 0;

  /** Simulation end time. */
  std::time_t myEndTime = 0;

  /** Grid for actors (2-d array of Actor pointers). */
  // TOOD: change to smart pointers
  Actor ***myWorld = nullptr;
};

#endif
