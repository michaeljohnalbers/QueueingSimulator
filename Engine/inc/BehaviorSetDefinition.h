#pragma once

/**
 * @file BehaviorSetDefinition.h
 * @brief BehaviorSet definition as defined in the plugin file.
 *
 * @author Michael Albers
 */

#include <string>
#include <vector>

namespace QS
{
  /**
   * Contains the definition for one behavior set as defined in a plugin
   * definition file.
   */
  class BehaviorSetDefinition
  {
    public:

    /**
     * Default constructor.
     */
    BehaviorSetDefinition() = delete;

    /**
     * Constructor.
     *
     * @param theName
     *          specific name of behavior
     * @param theBehaviors
     *          names of behaviors in this set
     */
    BehaviorSetDefinition(const std::string &theName,
                          const std::vector<std::string> &theBehaviors);

    /**
     * Copy constructor.
     */
    BehaviorSetDefinition(const BehaviorSetDefinition&) = default;

    /**
     * Move constructor.
     */
    BehaviorSetDefinition(BehaviorSetDefinition&&) = default;

    /**
     * Destructor.
     */
    ~BehaviorSetDefinition() = default;

    /**
     * Returns the list of behaviors this behavior set uses.
     *
     * @return behavior list
     */
    std::vector<std::string> getBehaviors() const noexcept;

    /**
     * Returns the name of the BehaviorSet.
     *
     * @return behavior set name
     */
    std::string getName() const noexcept;

    /**
     * Equality operator.
     *
     * @param theOther
     *          behavior set definition to compare against
     * @return true if the names are equal, false otherwise
     */
    bool operator==(const BehaviorSetDefinition &theOther) const noexcept;

    /**
     * Copy assignment operator.
     */
    BehaviorSetDefinition& operator=(const BehaviorSetDefinition&) = default;

    /**
     * Move assignment operator.
     */
    BehaviorSetDefinition& operator=(BehaviorSetDefinition&&) = default;

    protected:

    private:

    /** Name of the behavior set */
    const std::string myName;

    /** Names of all of the behaviors that this set uses. */
    const std::vector<const std::string> myBehaviors;
  };
}
