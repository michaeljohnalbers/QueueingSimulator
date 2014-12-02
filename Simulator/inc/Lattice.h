#ifndef _Lattice_h_
#define _Lattice_h_

/**
 * @file Lattice.h
 * @brief Represents the whole world and drives the simulation.
 *
 * @author Michael Albers
 */

#include <chrono>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <string>

#include <SimulatorTypedef.h>

class Bucket;
class Individual;

/**
 * The Lattice class is the primary class for the simulation. It defines and
 * manages the entire world. This class owns all of the Individuals which
 * comprise the simulation. It also splits the world into a number of Buckets.
 * These Buckets allow for parallelization of the simulation.
 */
class Lattice
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Lattice() = default;

  /**
   * Constructor.
   * @param theConfigFileName Name of configuration file.
   */
  Lattice(const std::string &theConfigFileName);

  /**
   * Copy constructor.
   * @param theObjectToCopy Object to copy.
   */
  Lattice(const Lattice &theObjectToCopy) = delete;

  /**
   * Destructor.
   */
  ~Lattice();

  /**
   * Assignment operator.
   * @param theRHS Object to copy.
   * @return This object.
   */
  Lattice& operator=(const Lattice &theRHS) = delete;

  /**
   * Runs the entire simulation. Does all setup/teardown work needed.
   */
  void runSimulation();

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  /**
   * Configures OpenMP.
   */
  void configureOpenMP();

  /**
   * Creates the Buckets which make up the world.
   */
  void createBuckets() throw (std::exception);

  /**
   * Creates all of the individuals which populate the world. The Individuals
   * are then placed into their initial Buckets.
   */
  void createIndividuals();

  /**
   * Determines the number of buckets and the dimension of myBuckets.
   */
  void determineLatticeGrid() throw (std::exception);

  /**
   * Updates one frame of the simulation.
   */
  void frameUpdate();

  /**
   * Generates a report on the completed simulation. The report is written
   * to stdout.
   */
  void generateReport();

  /**
   * Loads the configuration file.
   * @throws std::exception On any error loading the configuration file
   * or if there are illegal values in the file.
   */
  void loadConfigFile() throw (std::exception);

  /**
   * After all Buckets have been updated, rebuckets all Individuals which have
   * moved into a different Bucket.
   */
  void rebucketIndividuals();

  // ************************************************************
  // Private
  // ************************************************************
  private:

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

  /** Bucket configuration from config file. */
  std::string myBucketConfiguration;

  /** All Buckets which comprise the world, in a grid. */
  Bucket* **myBucketLattice = nullptr;

  /** All Buckets, but in a single dimension array. */
  Bucket* *myBuckets = nullptr;

  /** Name of the configuration file. */
  std::string myConfigFileName;

  /** Time of each frame, seconds. */
  float myFrameTime = 0.0001;

  /** Height of the world (as though viewed from above), meters. */
  int32_t myHeight = 0;

  /** All Individuals in the world. */
  Individual* *myIndividuals = nullptr;

  /** Number of columns in myBuckets. */
  int32_t myLatticeColumns = 0;

  /** Number of rows in myBuckets. */
  int32_t myLatticeRows = 0;

  /** Length of the world (as viewed from above), meters. */
  int32_t myLength = 0;

  /** Number of Buckets in the Lattice. */
  int32_t myNumberBuckets = 0;

  /** Total number of Individuals in the world. */
  int32_t myNumberIndividuals = 0;

  /** Engine for generating pseudo-random numbers. */
  std::default_random_engine myRandomEngine;

  /** Type of run. */
  QS::RunConfiguration myRunConfiguration;

  /** Simulation start time. */
  std::chrono::time_point<std::chrono::system_clock> mySimulationStartTime;

  /** Simulation stop time. */
  std::chrono::time_point<std::chrono::system_clock> mySimulationStopTime;

  /** Nesting level for OpenMP threads (for benchmarking). */
  int32_t myThreadNestingLevel = 0;

  /** Total number of frame updates. */
  uint64_t myTotalNumberFrames = 0;
};

#endif
