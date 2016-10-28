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
  class Sensable;

  /**
   * Basic definition of an individual within the simulation. This class
   * contains basic attributes of any individual such as mass, position, etc.
   *
   * This class is designed to be inherited from to add specific features and
   * capabilities for custom, user-defined actors.
   *
   * Actor local space is defined as follows:
   *   - The origin is at the center point of the Actor.
   *   - All units are in the same units as used in world space (radians and
   *   meters). So point (0,1) is one meter away.
   *   - Directly ahead/in front/forward is along the (1,0) vector. Directly
   *   behind is then (-1,0). The right (90 degree clockwise rotation) is (0,-1)
   *   and left is then (0,1).
   */
  class Actor : public PluginEntity, public DependencyManager<BehaviorSet>
  {
    public:

    /* Unit vector in Actor's default orientation. */
    static const Eigen::Vector2f DEFAULT_ORIENTATION;

    /**
     * Default constructor.
     */
    Actor() = delete;

    /**
     * Constructor. Initializes the the Actor from the properties map.
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
     *               (Example: "0.5 1.0 0.2"). If not given, defaults to (1,1,1)
     *            "orientation", angle, in radians, from the positive X axis (
     *               for example, an orientation of zero has the Actor pointed
     *               towards coordinate (1,0)). If not given, defaults to 0.
     *            "max speed", maximum speed of the Actor, in meters/second. If
     *               not given, defaults to -1. Negative values indicate no
     *               maximum. A zero value will force the Actor to be
     *               stationary.
     *            "max rotation", maximum angular speed of the Actor, in
     *               radians/second. If not given, defaults to -1. Negative
     *               values indicate no maximum. A zero value will prevent the
     *               Actor from moving. This maximum is direction neutral,
     *               meaning it is a maximum in both clockwise and
     *               counter-clockwise rotations.
     * @param theTag
     *          optional user-defined tag for differentiating Actors of the
     *          same type
     * @throws std::invalid_argument
     *           if map is missing any required propertu listed above, or if
     *           any property has an invalid value
     */
    Actor(const Properties &theProperties, const std::string &theTag);

    /**
     * Destructor.
     */
    virtual ~Actor() = default;

    /**
     * Takes the given vector, assumed to be in local coordinates, and returns
     * a new vectors which conforms to the Actor's maximum speed and rotational
     * speed.
     *
     * @param theVector
     *          vector in local coordinates (likely the return from
     *          calculateMotionVector)
     * @param theIntervalInSeconds
     *          duration in which the vector was generated. That is, the vector
     *          gives the meters or radians, the interval is the seconds in
     *          meters/second or radians/second.
     * @return new vector that fits within the Actor's defined maximum speed
     *         and rotational speed
     */
    virtual Eigen::Vector2f adjustVectorToMaximums(
      const Eigen::Vector2f &theVector,
      float theIntervalInSeconds) const noexcept;

    /**
     * Determines the vector the Actor wants to move. This function should not
     * actually update the Actor's position or any other physical attributes.
     * The engine will do that once it has done all of the actual physics and
     * collision detection involved with moving the Actor.
     *
     * The vector returned should be an Actor local coordinates.
     *
     * @param theSensable
     *          sensable data in the world
     * @return Vector to where the Actor should ("wants") be located, relative
     *         to the Actor's center point, after the update
     *         interval. So the units of the vector are meters/second.
     */
    virtual Eigen::Vector2f calculateMotionVector(
      const Sensable &theSensable);

    /**
     * Converts the given point (which is assumed to be in world space
     * coordinates) to local space coordinates.
     *
     * @param thePoint
     *          world space point to convert to local space
     * @return local space point
     */
    virtual Eigen::Vector2f convertPointToLocal(const Eigen::Vector2f &thePoint)
      const noexcept;

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
     * Returns the Actor's maximum rotational speed, in radians/second. If the
     * value is negative this indicates no maximum for the Actor.
     *
     * @return maximum rotational speed
     */
    float getMaximumRotationSpeed() const noexcept;

    /**
     * Returns the Actor's maximum speed, in meters/second. If the value is
     * negative this indicates no maximum for the Actor.
     *
     * @return maximum speed
     */
    float getMaximumSpeed() const noexcept;

    /**
     * Returns the Actor's orientation, in radians offset from (1,0) vector.
     *
     * @return orientation
     */
    float getOrientation() const noexcept;

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
     * Select the BehaviorSet to use for each update. Default implementation
     * simply picks the first set.
     *
     * @param theSensable
     *          sensable data in the world
     * @return BehaviorSet to use
     */
    virtual BehaviorSet* selectBehaviorSet(
      const Sensable &theSensable);

    /**
     * Sets the Actor's orientation angle offset from (1,0) vector. This
     * function will normalize the offet to [0,2*PI) regardless of the value
     * entered. (This function does not take the maximum rotational speed into
     * account when setting the orientation.)
     *
     * @param theOrientationAngle_radians
     *          orientation angle in radians
     */
    void setOrientation(float theOrientationAngle_radians) noexcept;

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
     * @param theDefault
     *          default value if the property isn't given or the conversion
     *          fails on an invalid optional property
     * @return property value of defined type
     * @throws std::invalid_argument if theRequired is true and the property
     *         doesn't exist.
     * @throws &lt;unknown&gt; whatever the converter might throw
     */
    template<class T>
    T getProperty(const std::string &thePropertyName, bool theRequired,
                  std::function<T(const std::string&)> theConversionFunction,
                  T theDefault = T())
      const;

    private:

    /** Actor color, in RGB suitable for OpenGL (each value is 0.0-1.0)*/
    Eigen::Vector3f myColor;

    /** Mass, in grams. */
    float myMass_grams;

    /**
     * Maximum rotational speed of the Actor, in radians/second. Negative
     * values indicate no maximum.
     */
    float myMaximumRotationSpeed_rs;

    /** Maximum speed of the Actor. Negative value indicates no maximum. */
    float myMaximumSpeed_ms;

    /** Angle offset from (1,0) vector. */
    float myOrientation_radians;

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
