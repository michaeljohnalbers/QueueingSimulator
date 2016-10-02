#pragma once

/**
 * @file BasicWalk.h
 * @brief About as simple a behavior set as you can get. Just guides the Actor
 *        in a straight line.
 *
 * @author Michael Albers
 */

#include "BehaviorSet.h"

namespace QS
{
  class BasicWalk : public BehaviorSet
  {
    public:

    /**
     * Default constructor.
     */
    BasicWalk() = delete;

    /**
     * Constructor
     *
     * @param theProperties
     *          all properties for this behavior set
     */
    BasicWalk(const Properties &theProperties);

    /**
     * Copy constructor.
     */
    BasicWalk(const BasicWalk&) = default;

    /**
     * Move constructor.
     */
    BasicWalk(BasicWalk&&) = default;

    /**
     * Destructor.
     */
    virtual ~BasicWalk() = default;

    /**
     * Evaluates a straight line walk.
     *
     * @param theActor
     *          Actor this Behavior is working on
     * @param theSensable
     *          sensable data from the environment
     * @return motion vector
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor,
                                     const Sensable &theSensable) override;

    /**
     * Copy assignment operator.
     */
    BasicWalk& operator=(const BasicWalk&) = default;

    /**
     * Move assignment operator.
     */
    BasicWalk& operator=(BasicWalk&&) = default;

    protected:

    private:
  };
}
