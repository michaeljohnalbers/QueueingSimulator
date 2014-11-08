#ifndef _Individual_h_
#define _Individual_h_

/**
 * @file Individual.h
 * @brief Base class used to represent a single actor in the simulation.
 *
 * Defines the base attributes and operations for all simulated actors.
 *
 * @author Michael Albers
 */

#include <cstdint>

#include <Position.h>

class NearestN;

/**
 * TODO: fill in
 */
class Individual
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /**
   * Default constructor.
   */
  Individual() = delete;

  /**
   * Constructor.
   * @param theBodyRadius Radius of Individual's body, in meters.
   * @param theID Unique ID
   * @param theMaximumAcceleration Maximum allowable acceleration,
   *        in meters per second squared.
   * @param theOrientation Orientation of individual. Angle from
   *        polar coordinate.
   * @param thePosition Initial position of Individual.
   */
  Individual(float theBodyRadius,
             int32_t theID,
             float theMaximumAcceleration,
             float theOrientation,
             const Position &thePosition);

  /**
   * Copy constructor.
   * @param Object to copy.
   */
  Individual(const Individual&) = delete;

  /**
   * Destructor.
   */
  virtual ~Individual();

  /**
   * Assignment operator.
   * @param Object to copy.
   * @return This object.
   */
  Individual& operator=(const Individual&) = delete;

  /**
   * Once per frame update function. Will update position, speed and
   * orientation of the Individual.
   * @param theNeighbors Nearest neighbors, used to determine position, speed
   * and orientation.
   * @param theFrameTime Correction factor. That is, the duration of the
   * frame, in seconds.
   */
  void frameUpdate(const NearestN &theNeighbors, float theFrameTime);

  /**
   * Returns the body radius of the individual.
   * @return Body radius, in meters.
   */
  float getBodyRadius() const;

  /**
   * Returns the unique ID of the individual.
   * @return Unique ID.
   */
  int32_t getID() const;

  /**
   * Returns the maximum allowable acceleration.
   * @return Maximum allowable acceleration, in meters per second squared.
   */
  float getMaximumAcceleration() const;

  /**
   * Returns the orientation of the individual.
   * @return Orientation of individual. Angle from polar coordinate.
   */
  float getOrientation() const;

  /**
   * Returns the Individual's current position.
   * @return Current position.
   */
  const Position& getPosition() const;

  /**
   * Returns the current speed.
   * @return Current speed, meters per second.
   */
  float getSpeed() const;

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  /** Body radius in meters. */
  float myBodyRadius;

  /** Unique ID. */
  int32_t myID;

  /** Maximum acceleration in meters per second squared. */
  float myMaximumAcceleration;

  /** Orientation of individual. Angle from polar coordinate. */
  float myOrientation;

  /** Location of Individual in the world. Cartesian coordinate. */
  Position myPosition;

  /** Current speed in meters/second. */
  float mySpeed = 0;
};

#endif
