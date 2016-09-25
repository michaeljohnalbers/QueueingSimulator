#pragma once

/**
 * @file Actor.h
 * @brief Basic definition of an individual within the simulation.
 *
 * @author Michael Albers
 */
#include <functional>
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
     *          Map of property name (key) to property value (value).
     *
     *          Required properties:
     *            "mass", value in grams
     *            "radius", value in meters
     *            "x", value in meters
     *            "y", value in meters.
     *          Optional properties:
     *            "color", space separated RGB values, each in the range 0.0-1.0
     *               (Example: "0.5 1.0 0.2")
     * @throws std::invalid_argument
     *           if map is missing any required propertu listed above, or if
     *           any property has an invalid value
     */
    Actor(const Properties &theProperties);

    /**
     * Destructor.
     */
    virtual ~Actor() = default;

    /**
     * Returns the color of the Actor. The vector elements are the R, G, B
     * values, respectively. Each is a range suitable for OpenGL (0.0-1.0).
     *
     * @return the color of the Actor
     */
    Eigen::Vector3f getColor() const noexcept;

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

    /**
     * Populates myColor from the "color" property.
     *
     * @throws std::invalid_argument on invalid RGB color.
     */
    void setColorFromProperty();

    /**
     * Populates myPosition from the "x" &amp; "y" properties.
     *
     * @throws std::invalid_argument on invalid coordinate.
     */
    void setPositionFromProperty();

    /**
     * Returns the property with the given name.
     *
     * @param thePropertyName
     *          property name
     * @param theRequired
     *          true if the property is required
     * @param theConversionFunction
     *          function to convert the string to the type T
     * @return property value of defined type, if theRequired if false and the
     * property doesn't exist, a type specific default is returned
     * @throws std::invalid_argument if theRequired is true and the property
     *         doesn't exist.
     * @throws &lt;unknown&gt; whatever the converter might throw
     */
    template<class T>
    T getProperty(const std::string &thePropertyName, bool theRequired,
                  std::function<T(const std::string&)> theConversionFunction)
      const;

    private:

    /** Actor color, in RGB suitable for OpenGL (each value is 0.0-1.0)*/
    Eigen::Vector3f myColor;

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
