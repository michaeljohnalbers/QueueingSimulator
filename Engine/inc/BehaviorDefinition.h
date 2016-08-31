#pragma once

/**
 * @file BehaviorDefinition.h
 * @brief Behavior definition as defined in the plugin file.
 *
 * @author Michael Albers
 */

#include <string>
#include <set>
#include "DefinitionPair.h"

namespace QS
{
  /**
   * Contains the definition for one behavior as defined in a plugin definition
   * file.
   */
  class BehaviorDefinition
  {
    public:

    /**
     * Default constructor.
     */
    BehaviorDefinition() = delete;

    /**
     * Constructor.
     *
     * @param theName
     *          specific name of behavior
     */
    BehaviorDefinition(const std::string &theName);

    /**
     * Copy constructor.
     */
    BehaviorDefinition(const BehaviorDefinition&) = default;

    /**
     * Move constructor.
     */
    BehaviorDefinition(BehaviorDefinition&&) = default;

    /**
     * Destructor.
     */
    ~BehaviorDefinition() = default;

    /**
     * Adds the given sensor, from the given plugin, to this Behavior.
     *
     * @param theSensor
     *          sensor to add
     * @param theSource
     *          source of the sensor
     */
    void addSensor(const std::string &theSensor,
                   const std::string &theSource) noexcept;

    /**
     * Returns the name of the type of Behavior.
     *
     * @return behavior type name
     */
    std::string getName() const noexcept;

    /**
     * Returns the list of sensors this behavior uses.
     *
     * @return sensor list
     */
    std::set<DefinitionPair> getSensors() const noexcept;

    /**
     * Equality operator.
     *
     * @param theOther
     *          behavior definition to compare against
     * @return true if the name and sensors are equal, false otherwise
     */
    bool operator==(const BehaviorDefinition &theOther) const noexcept;

    /**
     * Copy assignment operator.
     */
    BehaviorDefinition& operator=(const BehaviorDefinition&) = default;

    /**
     * Move assignment operator.
     */
    BehaviorDefinition& operator=(BehaviorDefinition&&) = default;

    protected:

    private:

    /** Name of the type of behavior */
    std::string myName;

    /** Names of all of the sensor that this behavior uses as input. */
    std::set<DefinitionPair> mySensors;
  };
}
