#pragma once

/**
 * @file BehaviorSet.h
 * @brief Groups a set of behaviors combining their outputs.
 *
 * @author Michael Albers
 */

#include "DependencyManager.h"
#include "EntityDependency.h"
#include "PluginEntity.h"
#include "Eigen/Core"

namespace QS
{
  class Actor;
  class Behavior;
  class Sensable;

  /**
   * A BehaviorSet groups a set of individual Behaviors together. Each Behavior
   * is evaluated and the results are combined. This allows more complex
   * Behaviors to be built from more modular pieces. This then allows
   * BehaviorSets to be swapped out for more appropriate sets during a
   * simulation.
   */
  class BehaviorSet : public PluginEntity, public DependencyManager<Behavior>
  {
    public:

    /**
     * Default constructor.
     */
    BehaviorSet() = delete;

    /**
     * Constructor
     *
     * @param theProperties
     *          all properties for this behavior set
     * @param theTag
     *          optional user-defined tag for differentiating BehaviorSets of
     *          the same type
     */
    BehaviorSet(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    BehaviorSet(const BehaviorSet&) = default;

    /**
     * Move constructor.
     */
    BehaviorSet(BehaviorSet&&) = default;

    /**
     * Destructor.
     */
    virtual ~BehaviorSet() = default;

    /**
     * Evaluates all contained Behaviors to create a set of steering forces that
     * are used to influence the motion of an Actor. (See QS::Behavior::evaluate
     * function for details on the steering forces being generated by
     * Behaviors.) This BehaviorSet then combines each steering force into a
     * single steering force which is then returned. How the BehaviorSet
     * combines the steering forces in entirely implementation dependent.
     *
     * The magnitude of this vector can essentially be as large (or small) as
     * the BehaviorSet wants it to be. The engine will clip this so as to not
     * exceed the maximums defined by the Actor. Put differently, the
     * BehaviorSet does not need to worry about making the steering force
     * magnitude comply with the Actor's defined maximums; unless the
     * BehaviorSet wants/needs to as part of its algorithm.
     *
     * This base implementation (i.e, this class, not any derived class) takes
     * all steering forces and averages them together.
     *
     * The vector returned must be an Actor local coordinates.
     *
     * @param theActor
     *          Actor this Behavior is working on
     * @param theSensable
     *          sensable data from the World
     * @return combined steering force vector
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor,
                                     const Sensable &theSensable);

    /**
     * Copy assignment operator.
     */
    BehaviorSet& operator=(const BehaviorSet&) = default;

    /**
     * Move assignment operator.
     */
    BehaviorSet& operator=(BehaviorSet&&) = default;

    /**
     * Populate all Sensors for all Behaviors.
     *
     * @param theSensable
     *          sensable data from the World
     */
    void populateSensors(const Sensable &theSensable) noexcept;

    protected:

    private:
  };
}
