#pragma once

/**
 * @file NullSensor.h
 * @brief Empty implementation of Sensor. Doesn't sense anything.
 *
 * @author Michael Albers
 */

#include "Sensor.h"

namespace QS
{
  class Sensable;

  /**
   * Implementation of Sensor to provide a concrete class. But it doesn't
   * actually do anything.
   */
  class NullSensor : public Sensor
  {
    public:

    /**
     * Default constructor.
     */
    NullSensor() = delete;

    /**
     * Constructor
     *
     * @see Sensor.h for parameter detail 
     */
    NullSensor(const Properties &theProperties,
               const std::string &theTag);

    /**
     * Destructor.
     */
    virtual ~NullSensor() = default;

    /**
     * Empty implementation
     */
    virtual void sense(const Sensable &theSensable) override;

    protected:

    private:
  };
}
