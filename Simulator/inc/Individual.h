#ifndef _Individual_h_
#define _Individual_h_

/**
 * @file Individual.h
 * @brief Base class used to represent a single actor in the simulation.
 *
 * @author Michael Albers
 */

#include <chrono>
#include <cstdint>
#include <memory>

#include <Eigen/Core>

class NearestN;

/**
 * All entities being simulated are an Individual. This class keeps track of
 * basic, common, data such as velocity, position, size and the like. This
 * class also provides a mechanism to update the velocity and position of
 * the Individual once per frame.
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
   * @param theMass Mass of the Individual.
   * @param theMaximumSpeed Maximum speed of the individual.
   * @param theMaximumAcceleration Maximum allowable acceleration,
   *        in meters per second squared.
   * @param theOrientation Orientation of individual. Angle from
   *        polar coordinate.
   * @param thePosition Initial position of Individual.
   * @param theRank Exit order.
   */
  Individual(float theMass,
             float theMaximumSpeed,
             float theMaximumAcceleration,
             float theOrientation,
             const Eigen::Vector2f &thePosition,
             int32_t theRank);

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
   * Determines if two indivdiuals have collided.
   * @return True if there is a collision, false otherwise.
   */
  bool collision(const Individual *theIndividual);

  /**
   * Once per frame update function. Will update position, speed and
   * orientation of the Individual.
   * @param theNeighbors Nearest neighbors, used to determine position, speed
   * and orientation.
   * @param theFrameTime Correction factor. That is, the duration of the
   * frame, in seconds.
   */
  void frameUpdate(std::shared_ptr<NearestN> theNeighbors, float theFrameTime);

  /**
   * Returns the body radius of the individual.
   * @return Body radius, in meters.
   */
  float getBodyRadius() const;

  /**
   * Returns the current velocity of the Individual.
   * @return Current velocity of the Individual, m/s.
   */
  const Eigen::Vector2f& getCurrentVelocity() const;

  /**
   * Returns the total distance traveled by the Individual.
   * @return Total distance traveled (meters).
   */
  float getDistanceTraveled() const;

  /**
   * Returns if this Individual has exited the world.
   * @return True if the individual has exited, false otherwise.
   */
  bool getExited() const;

  /**
   * Returns the time the individual exited the world.
   * @return Exit time.
   */
  const std::chrono::time_point<std::chrono::system_clock>& getExitTime() const;

  /**
   * Returns the maximum allowable acceleration.
   * @return Maximum allowable acceleration, in meters per second squared.
   */
  float getMaximumAcceleration() const;

  /**
   * Returns the maximum allowable force the Individual can exert.
   * @return Maximum allowable force, in Newtons.
   */
  float getMaximumForce() const;

  /**
   * Returns the maximum speed of the Individual.
   * @return Maximum speed, m/s.
   */
  float getMaximumSpeed() const;

  /**
   * Returns the orientation of the individual.
   * @return Orientation of individual. Angle from polar coordinate.
   */
  float getOrientation() const;

  /**
   * Returns the Individual's current position.
   * @return Current position.
   */
  const Eigen::Vector2f& getPosition() const;

  /**
   * Returns the rank of the individual.
   * @return Rank.
   */
  int32_t getRank() const;

  /**
   * Returns the current speed.
   * @return Current speed, meters per second.
   */
  float getSpeed() const;

  /**
   * Determines if the given point is within the body radius.
   * @param thePosition Point to check.
   * @return True if thePosition is within the body radius. False otherwise.
   */
  bool isPositionInRadius(const Eigen::Vector2f &thePosition);

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

  /** Current velocity vector, using Individual's local coordinate system. */
  Eigen::Vector2f myCurrentVelocity {0.0, 0.0};

  /** Total distance traveled. */
  float myDistanceTraveled = 0.0;

  /** Has the Individual exited the world? */
  bool myExited = false;

  /** Time Individual exited the world. */
  std::chrono::time_point<std::chrono::system_clock> myExitTime;

  /** Individual's mass, kg.*/
  float myMass;

  /** Maximum acceleration in meters per second squared. */
  float myMaximumAcceleration;

  /** Maximum force, in Newtons. */
  float myMaximumForce;

  /** Maximum speed of the Individual, in meters per second. */
  float myMaximumSpeed;

  /** Orientation of individual. Angle from polar coordinate. */
  float myOrientation;

  /** Location of Individual in the world. Cartesian coordinate. */
  Eigen::Vector2f myPosition;

  /** Rank of Individual. */
  int32_t myRank;

  /** Current speed in meters/second. */
  float mySpeed = 0.0;
};

#endif
