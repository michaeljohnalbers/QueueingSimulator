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

class Bucket;
class Individual;

/**
 * TODO: fill in
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
   * @param Object to copy.
   */
  Lattice(const Lattice&) = delete;

  /**
   * Destructor.
   */
  ~Lattice();

  /**
   * Assignment operator.
   * @param Object to copy.
   * @return This object.
   */
  Lattice& operator=(const Lattice&) = delete;

  /**
   * Runs the entire simulation. Does all setup/teardown work needed.
   */
  void runSimulation();

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

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

  /** All Buckets which comprise the world. */
  Bucket* myBuckets[];

  /** Name of the configuration file. */
  std::string myConfigFileName;

  /** All Individuals in the world. */
  Individual* myIndividuals[];

  /** Length of the world, meters. */
  int32_t myLength = 0;

  /** Total number of Individuals in the world. */
  int32_t myNumberIndividuals = 0;

  /** Engine for generating pseudo-random numbers. */
  std::default_random_engine myRandomEngine;

  /** Simulation start time. */
  std::chrono::time_point<std::chrono::system_clock> mySimulationStartTime;

  /** Width of the world, meters. */
  int32_t myWidth = 0;
};

#endif
