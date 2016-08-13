#pragma once

/**
 * @file Target.h
 * @brief Defines a single point in the world which can actors can move toward
 * or away from.
 *
 * @author Michael Albers
 */

#include <Eigen/Core>

namespace QS
{
  /**
   * A Target is simply a circular area within the simulation the Actors can
   * move towards or away from. Nothing more.
   */
  class Target
  {
    public:

    /**
     * Default constructor.
     */
    Target() = delete;

    /**
     * Constructor
     *
     * @param thePoint
     *          point of the target
     * @param theRadius
     *          size of the target, in meters, can be 0
     * @throws std::invalid_argument
     *          if theRadius is negative.
     * TODO: can thePoint values be negative?
     */
    Target(const Eigen::Vector2f &thePoint,
           const float theRadius);

    /**
     * Copy constructor.
     */
    Target(const Target&) = default;

    /**
     * Move constructor.
     */
    Target(Target &&) = default;

    /**
     * Destructor
     */
    virtual ~Target() = default;

    /**
     * Returns the target's point.
     *
     * @return target's point
     */
    Eigen::Vector2f getPoint() const noexcept;

    /**
     * Return the target's radius, in meters.
     *
     * @return radius, in meters
     */
    float getRadius() const noexcept;

    /**
     * Copy assignment operator.
     */
    Target& operator=(const Target&) = default;

    /**
     * Move assignment operator.
     */
    Target& operator=(Target &&) = default;

    protected:

    private:

    /** Target's location. */
    const Eigen::Vector2f myPoint;

    /** Radius of the target, in meters. */
    const float myRadius_m;
  };
}
