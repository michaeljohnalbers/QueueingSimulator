#pragma once

/**
 * @file OrderedLeaderFollow.h
 * @brief Follows the Actor with the closest higher rank.
 *
 * @author Michael Albers
 */

#include "Behavior.h"

namespace QS
{
  /**
   * Generates a steering force to follow the Actor with the closest higher
   * rank. If no higher Actor is nearby then a zero force is returned.
   */
  class OrderedLeaderFollow : public Behavior
  {
    public:

    /**
     * Default constructor.
     */
    OrderedLeaderFollow() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties for this OrderedLeaderFollow
     * @param theTag
     *          optional user-defined tag for differentiating
     *          OrderedLeaderFollows
     */
    OrderedLeaderFollow(const Properties &theProperties,
                        const std::string &theTag);

    /**
     * Copy constructor.
     */
    OrderedLeaderFollow(const OrderedLeaderFollow&) = default;

    /**
     * Move constructor.
     */
    OrderedLeaderFollow(OrderedLeaderFollow&&) = default;

    /**
     * Destructor.
     */
    virtual ~OrderedLeaderFollow() = default;

    /**
     * Generates a steering force to follow the Actor with the closest higher
     * rank. If no higher Actor is nearby then a zero force is returned.
     *
     * @param theActor
     *          Actor this OrderedLeaderFollow is working on
     * @return Steering force to apply to the Actor.
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor) override;

    /**
     * Copy assignment operator.
     */
    OrderedLeaderFollow& operator=(const OrderedLeaderFollow&) = default;

    /**
     * Move assignment operator.
     */
    OrderedLeaderFollow& operator=(OrderedLeaderFollow&&) = default;

    protected:

    private:
  };
}
