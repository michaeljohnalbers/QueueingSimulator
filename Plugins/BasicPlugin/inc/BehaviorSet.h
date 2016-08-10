#pragma once

/**
 * @file BehaviorSet.h
 * @brief Groups a set of behaviors combining their outputs.
 *
 * @author Michael Albers
 */

#include <map>
#include <string>

#include <Eigne/Core.h>

namespace QS
{
  class Behavior;
  class Sensable;

  /**
   * A BehaviorSet groups a set of individual Behaviors together. Each behavior
   * is evaluated and the results are combined. This allows more complex
   * behaviors to be built from more modular pieces. This then allows
   * BehaviorSets to be swapped out for more appropriate sets during a
   * simulation.
   */
  class BehaviorSet
  {
    public:

    /**
     * Default constructor.
     */
    BehaviorSet() = delete;

    /**
     * Constructor
     *
     * @param theBehaviors
     *          all behaviors that make up this set, key is the behavior name
     */
    BehaviorSet(const std::map<std::string, Behavior*> &theBehaviors);

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
     * Evaluates contained steering behaviors combining their result into a
     * single motion vector. The magnitude of the vector can be as large or
     * as small as needed. This function isn't responsible for making sure it
     * doesn't exceed any maximums.
     *
     * This base version simply takes all behaviors and adds the resulting
     * vectors together.
     *
     * @param theSensable
     *          sensable data from the environment
     * @return motion vector
     */
    virtual Eigen::Vec2f evaluate(const Sensable &theSensable);

    /**
     * Copy assignment operator.
     */
    BehaviorSet& operator=(const BehaviorSet&) = default;

    /**
     * Move assignment operator.
     */
    BehaviorSet& operator=(BehaviorSet&&) = default;

    protected:

    /** All behaviors for this set, keyed by name. */
    std::map<std::string, Behavior*> myBehaviors;

    private:
  };
}
