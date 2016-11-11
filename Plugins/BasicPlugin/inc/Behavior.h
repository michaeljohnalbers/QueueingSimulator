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
  class Actor;
  class Sensor;

  /**
   * A Behavior is a class which takes sensory input from the simulation world
   * and calculates a steering force for a single Actor. The Behavior calculates
   * this steering force as though it were the only Behavior acting upon the
   * Actor. In other words, the steering force should be the ideal vector that
   * the behavior can create. Likely this vector is going to be mixed in with
   * other vectors from other Behaviors to create a final vector that is
   * applied to the Actor. Refer to the 'evaluate' function for details about
   * what a steering force is.
   *
   * Sensory input comes from a set of Sensor objects. Each behavior defines
   * the sensors that it needs to calculate the steering force. The Queueing
   * Simulator engine will provide these sensors. The provided sensors will have
   * already "sensed" the environment.
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
     * @param theTag
     *          optional user-defined tag for differentiating Behaviors of the
     *          same type
     */
    Behavior(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    Behavior(const Behavior&) = default;

    /**
     * Move constructor.
     */
    Behavior(Behavior&&) = default;

    /**
     * Destructor.
     */
    virtual ~Behavior() = default;

    /**
     * Evaluates the steering behavior based on the dependency sensors in the
     * DependencyManager parent class. These sensors need to be already
     * populated (i.e., have "sensed" what they need).
     *
     * The returned vector is the steering "force" to be applied to the Actor's
     * currently velocity (after possibly being modified or mixed with other
     * steering forces in a BehaviorSet). The steering force vector should be
     * calculated without regard for any time interval. It should have the size
     * and direction to move the Actor to the desired location instantaneously.
     * The engine will truncate the force vector based on any time interval.
     *
     * The term "force" is not necessarily SI force (newton), though it will
     * more-or-less be treated like an actual force. Refer to the "Steering
     * Behaviors for Autonomous Characters" paper [Reynolds 99] for a deeper
     * discussion of this "force", how it is used and an example (namely the
     * "Seek" behavior) of when the "force" isn't really a force at all.
     *
     * The magnitude of this vector can essentially be as large (or small) as
     * the Behavior wants it to be. The engine will clip this so as to not
     * exceed the maximums defined by the Actor. Put differently, the Behavior
     * does not need to worry about making the steering force magnitude comply
     * with the Actor's defined maximums; unless the Behavior wants/needs to as
     * part of its algorithm.
     *
     * The vector returned must be an Actor local coordinates.
     *
     * This function will be called many, many times during the simulation. The
     * faster this function does its job, the faster the simulation will
     * run. However, there should be no parallelism used in this function. The
     * Queueing Simulator handles all parallelism.
     *
     * @param theActor
     *          Actor this Behavior is working on
     * @return Steering force to apply to the Actor.
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor) = 0;

    /**
     * Copy assignment operator.
     */
    Behavior& operator=(const Behavior&) = default;

    /**
     * Move assignment operator.
     */
    Behavior& operator=(Behavior&&) = default;

    protected:

    private:
  };
}
