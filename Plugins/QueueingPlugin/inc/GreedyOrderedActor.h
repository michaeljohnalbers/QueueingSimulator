#pragma once

/**
 * @file GreedyOrderedActor.h
 * @brief Actor which also has an ordering attribute.
 *
 * @author Michael Albers
 */

#include "OrderedActor.h"

namespace QS
{
  class GreedyOrdering;
  class SemiRationalOrdering;

  /**
   * Defines an Actor that orders greedily. That is, it rushes towards the
   * exit regardless of its rank.
   */
  class GreedyOrderedActor: public OrderedActor
  {
    public:

    /**
     * Default constructor.
     */
    GreedyOrderedActor() = delete;

    /**
     * Constructor. Initializes the the GreedyOrderedActor from the properties
     * map.
     *
     * @param theProperties
     *          Map of property name (key) to property value (value).
     *
     *          See OrderedActor.h for property list.
     *
     *          Optional properties:          
     *             "tolerated movement", distance between updates that is
     *               considered untolerable, in meters. Defaults to 0.005.
     *               Each update will be on the order of a few milliseconds.
     *             "patience time", amount of time, in seconds, of untolerable
     *               movement before the Actor will back away the exit. Time
     *               is added for every update where the amount of motion is
     *               less than the tolerable value ("tolerated movement"
     *               property). When this time amount is greater than this value
     *               the Actor starts backing up. Defaults to 20 seconds.
     * @param theTag
     *          optional user-defined tag for differentiating
                GreedyOrderedActors
     * @throws std::invalid_argument
     *           if map is missing any required property listed above, or if
     *           any property has an invalid value
     */
    GreedyOrderedActor(const Properties &theProperties,
                       const std::string &theTag);

    /**
     * Destructor.
     */
    virtual ~GreedyOrderedActor() = default;

    /**
     * @see Actor.h
     */
    virtual Eigen::Vector2f evaluate(const Sensable &theSensable) override;

    /**
     * Selects the Behavior Set to use.
     *
     * @param theSensable
     *          sensable data from the world
     */
    virtual BehaviorSet* selectBehaviorSet(const Sensable &theSensable)
      override;

   protected:

    private:

    enum class Phases
    {
      Greedy,
      SemiRational
    };

    /** Initial Behavior Set; crushes to the Exit. */
    GreedyOrdering *myGreedyBehaviorSet = nullptr;

    /** Number of seconds the Actor will be patient just standing around. */
    float myPatienceTime_s;

    /** Position at last update. */
    Eigen::Vector2f myPreviousPosition;

    /** Which phase of queuing is the Actor in? */
    Phases myPhase = Phases::Greedy;

    /** Behavior set for backing off from the exit. */
    SemiRationalOrdering *mySemiRationalBehaviorSet = nullptr;

    /** Amount of time spent not moving significantly. */
    float myTimeWithoutMovement = 0.0;

    /** Amount of distance, in meters, that is considered tolerable. */
    float myToleratedMovement_m;
  };
}
