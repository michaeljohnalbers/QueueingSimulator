#pragma once

/**
 * @file Sensor.h
 * @brief Base class for all sensors.
 *
 * @author Michael Albers
 */

#include <string>

#include "PluginEntity.h"

namespace QS
{
  class Sensable;

  /**
   * A sensor is an object which senses data within the simulation world. It
   * stores that information and is then used by steering behaviors in
   * determining Actor motion.
   *
   * Each sensor has a "type name". This is a loose concept. It doesn't not
   * necessarily mean the actual data type. Rather this is a name used in the
   * configuration files to tie a sensor to a steering behavior. A sensor
   * defines that it has a certain type name. Then a steering behavior
   * indicates that it needs a sensor of that type name.
   */
  class Sensor : public PluginEntity
  {
    public:

    /**
     * Default constructor.
     */
    Sensor() = delete;

    /**
     * Constructor
     *
     * @param theProperties
     *           properties for the sensor
     * @throws std::invalid_argument
     *           on empty type name
     */
    Sensor(const Properties &theProperties);

    /**
     * Destructor.
     */
    virtual ~Sensor() = default;

    /**
     * Populates the sensor with data from the sensable environment.
     *
     * @param theSensable
     *          object containing all that is able to be sensed within the
     *          simulation world.
     */
    virtual void sense(const Sensable &theSensable) = 0;

    protected:

    private:
  };
}
