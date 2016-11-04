#pragma once

/**
 * @file Target.h
 * @brief Defines a single position in the world which can actors can move
 *        toward or away from.
 *
 * @author Michael Albers
 */

#include "Eigen/Core"
#include "PluginEntity.h"

namespace QS
{
  /**
   * A Target is simply a circular area within the simulation the Actors can
   * move towards or away from. Nothing more.
   */
  class Target : public PluginEntity
  {
    public:

    /**
     * Default constructor.
     */
    Target() = delete;

    /**
     * Constructor. Initializes the the Target from the properties map.
     *
     * @param theProperties
     *          Map of property name (key) to property value (value).
     *
     *          Required properties:
     *            "radius", value in meters
     *            "x", value in meters
     *            "y", value in meters.
     * @param theTag
     *          optional user-defined tag for differentiating Targets of the
     *          same type
     * @throws std::invalid_argument
     *           if map is missing any required propertu listed above, or if
     *           any property has an invalid value
     */
    Target(const Properties &theProperties, const std::string &theTag);

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
     * Returns the target's position.
     *
     * @return target's position
     */
    Eigen::Vector2f getPosition() const noexcept;

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
    Eigen::Vector2f myPosition;

    /** Radius of the target, in meters. */
    float myRadius_m;
  };
}
