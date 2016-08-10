#pragma once

/**
 * @file Behavior.h
 * @brief Base class for all behaviors.
 *
 * @author Michael Albers
 */

#include <string>
#include <vector>

#include <Eigen/Core>

namespace QS
{
  class Sensor;

  /**
   * A behavior is a class which takes sensory input from the simulation world
   * and calculates a motion vector for a single Actor. The behavior calculates
   * this motion vector as though it were the only behavior acting upon the
   * Actor. In other words, the motion vector should be the ideal vector that
   * the behavior can create. Likely this vector is going to be mixed in with
   * other vectors from other behaviors to create a final vector that is
   * applied to the Actor.
   *
   * Sensory input comes from a set of Sensor objects. Each behavior defines
   * the sensors that it needs to calculate the motion vector. The Queueing
   * Simulator engine will provide these sensors. The provided sensors will have
   * already "sensed" the environment.
   *
   * The motion vector is calculated in the 'evaluate' function. This function
   * will be called many, many times. The faster this function does its job, the
   * faster the simulation will run. However, there should be no parallelism
   * used in this function. The Queueing Simulator handles all parallelism.
   */
  class Behavior
  {
    public:

    /**
     * Default constructor.
     */
    Behavior() = delete;

    /**
     * Constructor.
     *
     * @param theInputSensorTypes
     *          list of the names of sensors which are used as inputs to this
     *          behavior
     */
    Behavior(const std::vector<std::string> &theInputSensorTypes);

    /**
     * Copy constructor.
     */
    Behavior(const Behavior&) = delete;

    /**
     * Move constructor.
     */
    Behavior(Behavior&&) = delete;

    /**
     * Destructor.
     */
    virtual ~Behavior() = default;

    /**
     * Evaluates the steering behavior from the list of input sensors. Each
     * sensor must have already "sensed" what it needs to. This this behavior
     * need only retrieve the sensor's data.
     *
     * @param theSensors
     *          input sensors
     * @return vector used to influence the motion of the Actor
     */
    virtual Eigen::Vector2f evaluate(
      const std::vector<const Sensor*> theSensors) = 0;

    /**
     * Returns the list of input sensor types.
     *
     * @return list of input sensor types
     */
    std::vector<std::string> getInputSensorTypes() const;

    /**
     * Copy assignment operator.
     */
    Behavior& operator=(const Behavior&) = delete;

    /**
     * Move assignment operator.
     */
    Behavior& operator=(Behavior&&) = delete;

    protected:

    private:

    /**
     * Input sensor types.
     * TODO: this probably isn't sufficient. Might need info on which plugin the sensor originates and other data for the sensors.
     */
    const std::vector<std::string> myInputSensorTypes;
  };
}
