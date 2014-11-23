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
#include <SimulatorTypedef.h>

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
   * @param theMaximumSpeed Maximum speed of the individual. See
   *        documentation below for myMaximumSpeed for further
   *        details.
   * @param theMaximumAcceleration Maximum acceleration,
   *        in meters per second squared.
   * @param theOrientation Orientation of individual. Angle from
   *        polar coordinate.
   * @param thePosition Initial position of Individual.
   * @param theRank Exit order.
   * @param theRunConfiguration Configuration of this run of the simulator.
   */
  Individual(float theMass,
             float theMaximumSpeed,
             float theMaximumAcceleration,
             float theOrientation,
             const Eigen::Vector2f &thePosition,
             int32_t theRank,
             QS::RunConfiguration &theRunConfiguration);

  /**
   * Copy constructor.
   * @param theObjectToCopy Object to copy.
   */
  Individual(const Individual &theObjectToCopy) = delete;

  /**
   * Destructor.
   */
  virtual ~Individual();

  /**
   * Assignment operator.
   * @param theRHS Object to copy.
   * @return This object.
   */
  Individual& operator=(const Individual &theRHS) = delete;

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
   * Returns a string suitable for printing containing the immutable data
   * of the Individual.
   */
  std::string getBasicData() const;

  /**
   * Returns the body radius of the individual.
   * @return Body radius, in meters.
   */
  float getBodyRadius() const;

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
   * Returns the ideal time it will take to reach exit point moving in a
   * straight line if no other Individuals were in the way. (i.e., distance /
   * max speed)
   * @return Ideal straight line time (seconds).
   */
  float getIdealStraightLineTime() const;

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
   * Returns the maximum speed of the Individual. This is how fast the
   * Individual <i>wants</i> to walk, not how fast it <i>can</i>.
   * @return Maximum speed, m/s.
   */
  float getMaximumSpeed() const;

  /**
   * Returns the orientation of the individual.
   * @return Orientation of individual. Angle from polar coordinate.
   */
  float getOrientation() const;

  /**
   * Returns the Individual's original (starting) position.
   * @return Original position.
   */
  const Eigen::Vector2f getOriginalPosition() const;

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
   * Returns the current velocity of the Individual.
   * @return Current velocity of the Individual, m/s.
   */
  const Eigen::Vector2f& getVelocity() const;

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
  const float myBodyRadius;

  /** Total distance traveled. */
  float myDistanceTraveled = 0.0;

  /** Has the Individual exited the world? */
  bool myExited = false;

  /** Time Individual exited the world. */
  std::chrono::time_point<std::chrono::system_clock> myExitTime;

  /**
   *  Ideal time it will take to reach exit point moving in a straight line
   *  if no other Individuals were in the way. (i.e., distance / max speed)
   */
  float myIdealStraightLineTime;

  /** Individual's mass, kg.*/
  const float myMass;

  /** Maximum acceleration in meters per second squared. */
  const float myMaximumAcceleration;

  /** Maximum force, in Newtons. */
  const float myMaximumForce;

  /**
   * Maximum speed of the Individual, in meters per second.
   * This could simply be calculated based off force and mass and some time
   * value. However, this should be considered the maximum speed the
   * Individual <i>wants</i> to walk, not how fast it <i>can</i>.
   */
  const float myMaximumSpeed;

  /** Orientation of individual. Angle from polar coordinate. */
  float myOrientation;

  /** Original position of the Individual. Cartesian coordinate. */
  Eigen::Vector2f myOriginalPosition;

  /** Location of Individual in the world. Cartesian coordinate. */
  Eigen::Vector2f myPosition;

  /** Rank of Individual. */
  const int32_t myRank;

  /** Configuration of this simulation run. */
  const QS::RunConfiguration myRunConfiguration;

  /** Current speed in meters/second. */
  float mySpeed = 0.0;

  /** Current velocity vector, using Individual's local coordinate system. */
  Eigen::Vector2f myVelocity {0.0, 0.0};
};

#endif
