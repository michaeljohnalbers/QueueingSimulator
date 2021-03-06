#pragma once

/**
 * @file BehaviorSetDefinition.h
 * @brief BehaviorSet definition as defined in the plugin file.
 *
 * @author Michael Albers
 */

#include <string>
#include <set>
#include "PluginDependencySet.h"

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
     */
    BehaviorSetDefinition(const std::string &theName);

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
     * Adds the given behavior, from the given plugin, to this behavior set.
     *
     * @param theBehavior
     *          behavior to add
     * @param theSource
     *          source of the behavior
     * @param theTag
     *          optional identifier to allow multiple Behaviors of the same
     *          type for a single BehaviorSet
     */
    void addBehavior(const std::string &theBehavior,
                     const std::string &theSource,
                     const std::string &theTag) noexcept;

    /**
     * Returns the list of behaviors this behavior set uses.
     *
     * @return behavior list
     */
    std::set<PluginDependencySet> getBehaviors() const noexcept;

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
     * @return true if the name & behaviors are equal, false otherwise
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
    std::string myName;

    /** Names of all of the behaviors that this set uses. */
    std::set<PluginDependencySet> myBehaviors;
  };
}
