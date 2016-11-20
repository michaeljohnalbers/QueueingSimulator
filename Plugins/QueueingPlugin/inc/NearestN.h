#pragma once

/**
 * @file NearestN.h
 * @brief Finds N Actors within a radius of the current Actor.
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <vector>
#include "Sensor.h"

namespace QS
{
  class Actor;

  /**
   * Finds up to N Actors within a specified radius of the current Actor.
   */
  class NearestN : public Sensor
  {
    public:

    /**
     * Default constructor.
     */
    NearestN() = delete;

    /**
     * Constructor
     *
     * @param theProperties
     *          Map of property name (key) to property value (value).
     *
     *          Required properties:
     *            "N", number of actors to find, unsigned integer
     *            "radius", radius of the Actor to search, floating point,
     *              in meters, must be &gte; zero.
     * @param theTag
     *          optional user-defined tag for differentiating Sensors of the
     *          same type
     * @throws std::invalid_argument
     *           if map is missing any required propertu listed above, or if
     *           any property has an invalid value
     */
    NearestN(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    NearestN(const NearestN&) = default;

    /**
     * Move constructor.
     */
    NearestN(NearestN&&) = default;

    /**
     * Destructor.
     */
    virtual ~NearestN() = default;

    /**
     * Returns the found Actors.
     *
     * @return actor list
     */
    const std::vector<const Actor*>& getActors() const noexcept;

    /**
     * Returns the maximum number of Actors to find.
     *
     * @return number of Actors
     */
    uint32_t getN() const noexcept;

    /**
     * Returns the search radius, in meters.
     *
     * @return radius
     */
    float getRadius() const noexcept;

    /**
     * Copy assignment operator.
     */
    NearestN& operator=(const NearestN&) = default;

    /**
     * Move assignment operator
     */
    NearestN& operator=(NearestN&&) = default;

    /**
     * Finds up to N Actors within a specified radius of the current Actor.
     *
     * @param theSensable
     *          object containing all that is able to be sensed within the
     *          simulation world.
     */
    virtual void sense(const Sensable &theSensable) override;

    protected:

    private:

    /**  Nearest N Actors. */
    std::vector<const Actor*> myActors;

    /** Number of Actors to find. */
    uint32_t myN = 0;

    /** Radius to search. */
    float myRadius_m = 0.0;
  };
}
