#ifndef _ActorGenerator_h_
#define _ActorGenerator_h_

/**
 * @file ActorGenerator.h
 * @brief Class used to generate a set of actors.
 * @author Michael Albers
 */

#include <Actor.h>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * This class creates a set of actors to populate the world. The class can also
 * write the actor data to a file and read in previously created files.
 */
class ActorGenerator
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  ActorGenerator() = delete;

  /**
   * Constructor. Generates actors from the given parameters.
   * @param theNumberActors Number of Actors to generate
   * @param theNumberRows Number of rows in the world.
   * @param theNumberColumns Number of columns in the world.
   * @param theSeed Random number generator for generating actor positions.
   */
  ActorGenerator(int32_t theNumberActors,
		 int32_t theNumberRows,
		 int32_t theNumberColumns,
		 int32_t theSeed);

  /**
   * Constructor. Generates actors from a file written by this class.
   * @param theInputFile Input file with world/actor data.
   */
  ActorGenerator(const std::string &theInputFile);

  /**
   * Returns the list of all the actors.
   * @return Actor list.
   */
  const std::vector<Actor>& getActors() const;

  /**
   * Returns the number of columns in the world.
   * @return Number of columns in the world.
   */
  int32_t getNumberColumns() const;

  /**
   * Returns the number of rows in the world.
   * @return Number of rows in the world.
   */
  int32_t getNumberRows() const;

  /**
   * Generates all of the actors.
   * @throw std::logic_error On any error while generating the actors.
   */
  void generateActors() throw (std::logic_error);

  /**
   * Writes actor data to a file which can later be used for consumption by
   * this class. Must first call generateActors.
   * @param theFileName Name of the file to write to.
   * @throws std::logic_error On any error writing the input file.
   */
  void writeInputFile(const std::string &theFileName) const
    throw (std::logic_error);

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  typedef enum
  {
    PARAMETERS,
    FILE
  } GenerationType;

  /**
   * Generates the actors based on the contents of the file.
   * @throw std::logic_error
   *   On any error reading the file or invalid data from the file.
   */
  void generateActorsFromFile() throw (std::logic_error);

  /**
   * Generates the actors based on the parameters given to the constructor.
   * @throw std::logic_error On invalid parameters.
   */
  void generateActorsFromParameters() throw (std::logic_error);

  /**
   * Verifies that the number of actors and world size are compatible.
   * @throw std::logic_error
   *   If the number of actors and world size are not compatible.
   */
  void verifyActorsAndWorldSize() throw (std::logic_error);

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** List of Actors */
  std::vector<Actor> myActors;
  /** How to generate the actors. */
  GenerationType myGenerationType;
  /** Input file with actor/world data. */
  std::string myInputFile;
  /** Number of actors in the world. */
  int32_t myNumberActors = 0;
  /** Number of columns in the world. */
  int32_t myNumberColumns = 0;
  /** Number of rows in the world. */
  int32_t myNumberRows = 0;
  /** Seed for generating actor positions. */
  int32_t mySeed = 0;
};

#endif
