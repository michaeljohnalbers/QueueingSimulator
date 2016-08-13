#pragma once

/**
 * @file BehaviorDefinition.h
 * @brief Behavior definition as defined in the plugin file.
 *
 * @author Michael Albers
 */

#include <string>
#include <vector>

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
     * @param theSensors
     *          names of sensors used for input
     */
    BehaviorDefinition(const std::string &theName,
                       const std::vector<std::string> &theSensors);

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
    std::vector<std::string> getSensors() const noexcept;

    /**
     * Equality operator.
     *
     * @param theOther
     *          behavior definition to compare against
     * @return true if the names are equal, false otherwise
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
    const std::string myName;

    /** Names of all of the sensor that this behavior uses as input. */
    const std::vector<std::string> mySensors;
  };
}
