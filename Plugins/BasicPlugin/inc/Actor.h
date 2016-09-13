#pragma once

/**
 * @file Actor.h
 * @brief Basic definition of an individual within the simulation.
 *
 * @author Michael Albers
 */

#include "DependencyManager.h"
#include "EntityDependency.h"
#include "PluginEntity.h"
#include "Eigen/Core"

namespace QS
{
  class BehaviorSet;

  /**
   * Basic definition of an individual within the simulation. This class
   * contains basic attributes of any individual such as mass, position, etc.
   *
   * This class is designed to be inherited from to add specific features and
   * capabilities for custom, user-defined actors.
   */
  class Actor : public PluginEntity, public DependencyManager<BehaviorSet>
  {
    public:

    /**
     * Default constructor.
     */
    Actor() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          Map of property name (key) to property value (value). Must
     *          contain "mass", value in grams, and "radius", value in meters.
     * @throws std::invalid_argument
     *           if map is missing "mass" and "radius" properties, or if either
     *           of those properties have invalid values
     */
    Actor(const Properties &theProperties);

    /**
     * Destructor.
     */
    virtual ~Actor() = default;

    /**
     * Returns the Actor's mass, in grams.
     *
     * @return mass
     */
    float getMass() const noexcept;

    /**
     * Returns the Actor's position.
     *
     * @return Actor's position
     */
    Eigen::Vector2f getPosition() const noexcept;

    /**
     * Returns the radius of the actor, in meters.
     *
     * @return radius
     */
    float getRadius() const noexcept;

    /**
     * Returns the velocity vector. Origin at Actor's current location.
     * Magnitude is in meters/second.
     *
     * @return velocity vector.
     */
    Eigen::Vector2f getVelocity() const noexcept;

    /**
     * Sets the Actor's position.
     *
     * @param thePosition
     *          new position
     */
    void setPosition(const Eigen::Vector2f &thePosition) noexcept;

    /**
     * Sets the Actor's velocity. Origin at Actor's current location.
     *
     * @param theVelocity
     *          new velocity vector, magnitude will be in meters/second
     */
    void setVelocity(const Eigen::Vector2f &theVelocity) noexcept;

    protected:

    private:

    /** Mass, in grams. */
    float myMass_grams;

    /** Location of Individual in the world. Cartesian coordinate. */
    Eigen::Vector2f myPosition;

    /** Actor's radius, in meters. */
    float myRadius_m;

    /**
     * Velocity vector, origin at Actor's current location. Measured
     * in meters per second.
     */
    Eigen::Vector2f myVelocity_ms;
  };
}
