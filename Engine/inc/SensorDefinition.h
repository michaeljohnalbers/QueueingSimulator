#pragma once

/**
 * @file SensorDefinition.h
 * @brief Sensor definition as defined in the plugin file.
 *
 * @author Michael Albers
 */

#include <string>

namespace QS
{
  /**
   * Contains the definition for one type of sensor as defined in a plugin
   * definition file.
   */
  class SensorDefinition
  {
    public:

    /**
     * Default constructor.
     */
    SensorDefinition() = delete;

    /**
     * Constructor.
     *
     * @param theName
     *          specific name of Sensor type
     */
    SensorDefinition(const std::string &theName);

    /**
     * Copy constructor.
     */
    SensorDefinition(const SensorDefinition&) = default;

    /**
     * Move constructor.
     */
    SensorDefinition(SensorDefinition&&) = default;

    /**
     * Destructor.
     */
    ~SensorDefinition() = default;

    /**
     * Returns the name of the type of Sensor.
     *
     * @return sensor type name
     */
    std::string getName() const noexcept;

    /**
     * Equality operator.
     *
     * @param theOther
     *          sensor definition to compare against
     * @return true if the type names are equal, false otherwise
     */
    bool operator==(const SensorDefinition &theOther) const noexcept;

    /**
     * Copy assignment operator.
     */
    SensorDefinition& operator=(const SensorDefinition&) = default;

    /**
     * Move assignment operator.
     */
    SensorDefinition& operator=(SensorDefinition&&) = default;

    protected:

    private:

    /** Name of the type of sensor */
    std::string myName;
  };
}
