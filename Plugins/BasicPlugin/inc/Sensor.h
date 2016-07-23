#pragma once

/**
 * @file Sensor.h
 * @brief Base class for all sensors.
 *
 * @author Michael Albers
 */

#include <string>

namespace QS
{
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
  class Sensor
  {
    public:

    /**
     * Default constructor.
     */
    Sensor() = delete;

    /**
     * Constructor
     *
     * @param theTypeName
     *           name of the type of sensor that this is
     */
    Sensor(const std::string &theTypeName);

    /**
     * Destructor.
     */
    virtual ~Sensor() = default;

    /**
     * Returns the name of the type of this sensor.
     *
     * @return type name
     */
    std::string getTypeName() const;

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

    /** Sensor type name*/
    const std::string myTypeName;
  };
}
