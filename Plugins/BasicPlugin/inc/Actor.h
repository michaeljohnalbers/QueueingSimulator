#pragma once

/**
 * @file Actor.h
 * @brief Basic definition of an individual within the simulation.
 *
 * @author Michael Albers
 */

#include <map>
#include <string>

#include <Eigen/Core>

namespace QS
{
  /**
   * Basic definition of an individual within the simulation. This class
   * contains basic attributes of any individual such as mass, position, etc.
   *
   * This class is designed to be inherited from to add specific features and
   * capabilities for custom, user-defined actors.
   */
  class Actor
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
     *           if map is missing "mass" and "radius" properties.
     */
    Actor(const std::map<std::string, std::string> &theProperties);

    /**
     * Destructor.
     */
    virtual ~Actor() = default;

    /**
     * Returns the Actor's mass, in grams.
     *
     * @return mass
     */
    float getMass() const;

    /**
     * Returns the Actor's position.
     *
     * @return Actor's position
     */
    Eigen::Vector2f getPosition() const;

    /**
     * Returns the radius of the actor, in meters.
     *
     * @return radius
     */
    float getRadius() const;

    /**
     * Returns the velocity vector. Origin at Actor's current location.
     * Magnitude is in meters/second.
     *
     * @return velocity vector.
     */
    Eigen::Vector2f getVelocity() const;

    /**
     * Sets the Actor's position.
     *
     * @param thePosition
     *          new position
     */
    void setPosition(const Eigen::Vector2f &thePosition);

    /**
     * Sets the Actor's velocity. Origin at Actor's current location.
     *
     * @param theVelocity
     *          new velocity
     */
    void setVelocity(const Eigen::Vector2f &theVelocity);

    protected:

    private:

    /** Mass, in grams. */
    const float myMass_grams;

    /** Location of Individual in the world. Cartesian coordinate. */
    Eigen::Vector2f myPosition;

    /** Actor's radius, in meters. */
    const float myRadius_m;

    /**
     * Velocity vector, origin at Actor's current location. Measured
     * in meters per second.
     */
    Eigen::Vector2f myVelocity_ms;
  };
}
