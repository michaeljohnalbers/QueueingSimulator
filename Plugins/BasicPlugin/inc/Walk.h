#pragma once

/**
 * @file Walk.h
 * @brief Straight line motion.
 *
 * @author Michael Albers
 */

#include "Behavior.h"

namespace QS
{
  /**
   * Simple behavior to direct Actor to move in a straight line.
   */
  class Walk : public Behavior
  {
    public:

    /**
     * Default constructor.
     */
    Walk() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties for this Walk
     */
    Walk(const Properties &theProperties);

    /**
     * Copy constructor.
     */
    Walk(const Walk&) = delete;

    /**
     * Move constructor.
     */
    Walk(Walk&&) = delete;

    /**
     * Destructor.
     */
    virtual ~Walk() = default;

    /**
     * Directs Actor to move in a straight line.
     *
     * @return vector used to influence the motion of the Actor
     */
    virtual Eigen::Vector2f evaluate() override;

    /**
     * Copy assignment operator.
     */
    Walk& operator=(const Walk&) = delete;

    /**
     * Move assignment operator.
     */
    Walk& operator=(Walk&&) = delete;

    protected:

    private:
  };
}
