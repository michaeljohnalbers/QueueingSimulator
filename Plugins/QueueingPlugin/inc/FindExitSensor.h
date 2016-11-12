#pragma once

/**
 * @file FindExitSensor.h
 * @brief Simple sensor to get an Exit.
 *
 * @author Michael Albers
 */

#include "Sensor.h"

namespace QS
{
  class Exit;

  /**
   * Pulls the first Exit out of the Sensable data, if there are Exits in the
   * Sensable data.
   */
  class FindExitSensor : public Sensor
  {
    public:

    /**
     * Default constructor.
     */
    FindExitSensor() = delete;

    /**
     * Constructor
     *
     * @param theProperties
     *           properties for the sensor
     * @param theTag
     *          optional user-defined tag for differentiating Sensors of the
     *          same type
     */
    FindExitSensor(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    FindExitSensor(const FindExitSensor&) = default;

    /**
     * Move constructor.
     */
    FindExitSensor(FindExitSensor&&) = default;

    /**
     * Destructor.
     */
    virtual ~FindExitSensor() = default;

    /**
     * Returns the Exit or nullptr if no Exit was found.
     *
     * @return exit or nullptr
     */
    const Exit* getExit() const noexcept;

    /**
     * Copy assignment operator.
     */
    FindExitSensor& operator=(const FindExitSensor&) = default;

    /**
     * Move assignment operator
     */
    FindExitSensor& operator=(FindExitSensor&&) = default;

    /**
     * Pulls an Exit from the Sensable data.
     *
     * @param theSensable
     *          object containing all that is able to be sensed within the
     *          simulation world.
     */
    virtual void sense(const Sensable &theSensable) override;

    protected:

    private:

    /** Exit from Sensable. */
    const Exit *myExit = nullptr;
  };
}
