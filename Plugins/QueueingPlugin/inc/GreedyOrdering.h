#pragma once

/**
 * @file GreedyOrdering.h
 * @brief Orders the Actor in a greedy fashion, jamming towards the Exit
 * regardless of order.
 *
 * @author Michael Albers
 */

#include "BehaviorSet.h"

namespace QS
{
  class CollisionAvoidance;
  class ExitSeek;
  class NearExitArrival;

  /**
   * Orders the Actor in a greedy fashion, jamming towards the Exit regardless
   * of order.
   */
  class GreedyOrdering : public BehaviorSet
  {
    public:

    /**
     * Default constructor.
     */
    GreedyOrdering() = delete;

    /**
     * Constructor
     *
     * @param theProperties
     *          all properties for this behavior set
     * @param theTag
     *          optional user-defined tag for differentiating GreedyOrderings
     */
    GreedyOrdering(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    GreedyOrdering(const GreedyOrdering&) = default;

    /**
     * Move constructor.
     */
    GreedyOrdering(GreedyOrdering&&) = default;

    /**
     * Destructor.
     */
    virtual ~GreedyOrdering() = default;

    /**
     *
     * @param theActor
     *          Actor this Behavior is working on
     * @param theSensable
     *          sensable data from the environment
     * @return combined steering force vector
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor,
                                     const Sensable &theSensable) override;

    /**
     * Copy assignment operator.
     */
    GreedyOrdering& operator=(const GreedyOrdering&) = default;

    /**
     * Move assignment operator.
     */
    GreedyOrdering& operator=(GreedyOrdering&&) = default;

    protected:

    private:

    /** Collision avoidance behavior */
    CollisionAvoidance *myCollisionAvoidance = nullptr;

    /** Move towards exit */
    ExitSeek *myExitSeek = nullptr;

    /** Near-exit arrival behavior */
    NearExitArrival *myNearExitArrival = nullptr;
  };
}
