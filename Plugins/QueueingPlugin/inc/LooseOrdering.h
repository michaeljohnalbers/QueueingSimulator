#pragma once

/**
 * @file LooseOrdering.h
 * @brief Orders Actors according to the nearest Actors orders and exits as
 * quickly as possible.
 *
 * @author Michael Albers
 */

#include "BehaviorSet.h"

namespace QS
{
  /**
   */
  class LooseOrdering : public BehaviorSet
  {
    public:

    /**
     * Default constructor.
     */
    LooseOrdering() = delete;

    /**
     * Constructor
     *
     * @param theProperties
     *          all properties for this behavior set
     * @param theTag
     *          optional user-defined tag for differentiating LooseOrderings
     */
    LooseOrdering(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    LooseOrdering(const LooseOrdering&) = default;

    /**
     * Move constructor.
     */
    LooseOrdering(LooseOrdering&&) = default;

    /**
     * Destructor.
     */
    virtual ~LooseOrdering() = default;

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
    LooseOrdering& operator=(const LooseOrdering&) = default;

    /**
     * Move assignment operator.
     */
    LooseOrdering& operator=(LooseOrdering&&) = default;

    protected:

    private:
  };
}
