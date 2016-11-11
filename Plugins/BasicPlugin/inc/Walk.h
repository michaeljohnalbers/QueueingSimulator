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
     *          properties for this Behavior
     *
     *          Optional properties:
     *            "speed", speed to walk in meters per second. If not
     *            specified, 1.0 is used.
     * @throw std::invalid_argument
     *          if "speed" property cannot be converted to a float
     */
    Walk(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    Walk(const Walk&) = default;

    /**
     * Move constructor.
     */
    Walk(Walk&&) = default;

    /**
     * Destructor.
     */
    virtual ~Walk() = default;

    /**
     * Directs Actor to move in a straight line at a fixed speed.
     *
     * @param theActor
     *          Actor this Behavior is working on
     * @param theIntervalInSeconds
     *          amount of time elapsed since last simulation update
     * @return steering force to direct the walking
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor) override;

    /**
     * Copy assignment operator.
     */
    Walk& operator=(const Walk&) = default;

    /**
     * Move assignment operator.
     */
    Walk& operator=(Walk&&) = default;

    protected:

    /** Speed, in meters per second. */
    float mySpeed_ms;

    private:
  };
}
