#pragma once

/**
 * @file Behavior.h
 * @brief Base class for all behaviors.
 *
 * @author Michael Albers
 */

#include <string>
#include <vector>

#include "DependencyManager.h"
#include "PluginEntity.h"
#include "Eigen/Core"

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
  class Behavior : public PluginEntity, public DependencyManager<Sensor>
  {
    public:

    /**
     * Default constructor.
     */
    Behavior() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties for this Behavior
     */
    Behavior(const Properties &theProperties);

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
     * Evaluates the steering behavior based on the dependency sensors in the
     * DependencyManager parent class. These sensors need to be already
     * populated (i.e., have "sensed" what they need).
     *
     * @return vector used to influence the motion of the Actor
     */
    virtual Eigen::Vector2f evaluate() = 0;

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
  };
}
