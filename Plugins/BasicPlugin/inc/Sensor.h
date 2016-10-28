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
     * @param theTag
     *          optional user-defined tag for differentiating Sensors of the
     *          same type
     */
    Sensor(const Properties &theProperties, const std::string &theTag);

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
