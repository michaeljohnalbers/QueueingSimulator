#pragma once

/**
 * @file SemiRationalOrdering.h
 * @brief Used after GreedyOrdering. Backs the Actor off from the Exit, unless
 * it's the Actor that should exit.
 *
 * @author Michael Albers
 */

#include "BehaviorSet.h"

namespace QS
{
  class CollisionAvoidance;
  class ExitFlee;
  class ExitSeek;
  class Separation;

  /**
   * Used after GreedyOrdering. Backs the Actor off from the Exit, unless it's
   * the Actor that should exit.
   */
  class SemiRationalOrdering : public BehaviorSet
  {
    public:

    /**
     * Default constructor.
     */
    SemiRationalOrdering() = delete;

    /**
     * Constructor
     *
     * @param theProperties
     *          all properties for this behavior set
     * @param theTag
     *          optional user-defined tag for differentiating
     *          SemiRationalOrderings
     */
    SemiRationalOrdering(const Properties &theProperties,
                         const std::string &theTag);

    /**
     * Copy constructor.
     */
    SemiRationalOrdering(const SemiRationalOrdering&) = default;

    /**
     * Move constructor.
     */
    SemiRationalOrdering(SemiRationalOrdering&&) = default;

    /**
     * Destructor.
     */
    virtual ~SemiRationalOrdering() = default;

    /**
     * Moves the Actor away from the Exit, unless its the Actor to exit.
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
    SemiRationalOrdering& operator=(const SemiRationalOrdering&) = default;

    /**
     * Move assignment operator.
     */
    SemiRationalOrdering& operator=(SemiRationalOrdering&&) = default;

    protected:

    private:

    /** Collision avoidance behavior */
    CollisionAvoidance *myCollisionAvoidance = nullptr;

    /** Move away from the exit. */
    ExitFlee *myExitFlee = nullptr;

    /** Move towards the exit. */
    ExitSeek *myExitSeek = nullptr;

    /** Separates Actors from one another. */
    Separation *mySeparation = nullptr;
  };
}
