#pragma once

/**
 * @file ActorDefinition.h
 * @brief Actor definition as defined in the plugin file.
 *
 * @author Michael Albers
 */

#include <string>
#include <set>
#include "PluginDependencySet.h"

namespace QS
{
  /**
   * Contains the definition for one name of actor as defined in a plugin
   * definition file.
   */
  class ActorDefinition
  {
    public:

    /**
     * Default constructor.
     */
    ActorDefinition() = delete;

    /**
     * Constructor.
     *
     * @param theName
     *          specific name of Actor
     */
    ActorDefinition(const std::string &theName);

    /**
     * Copy constructor.
     */
    ActorDefinition(const ActorDefinition&) = default;

    /**
     * Move constructor.
     */
    ActorDefinition(ActorDefinition&&) = default;

    /**
     * Destructor.
     */
    ~ActorDefinition() = default;

    /**
     * Adds the given behavior set, from the given plugin, to this Actor.
     *
     * @param theBehaviorSet
     *          behavior set to add
     * @param theSource
     *          source of the behavior set
     * @param theTag
     *          optional identifier to allow multiple BehaviorSets of the same
     *          type for a single Actor
     */
    void addBehaviorSet(const std::string &theBehaviorSet,
                        const std::string &theSource,
                        const std::string &theTag) noexcept;

    /**
     * Returns the list of behavior sets this actor uses.
     *
     * @return behavior set list
     */
    std::set<PluginDependencySet> getBehaviorSets() const noexcept;

    /**
     * Returns the name of the name of Actor.
     *
     * @return actor name name
     */
    std::string getName() const noexcept;

    /**
     * Equality operator.
     *
     * @param theOther
     *          actor definition to compare against
     * @return true if the name is equal, false otherwise
     */
    bool operator==(const ActorDefinition &theOther) const noexcept;

    /**
     * Copy assignment operator.
     */
    ActorDefinition& operator=(const ActorDefinition&) = default;

    /**
     * Move assignment operator.
     */
    ActorDefinition& operator=(ActorDefinition&&) = default;

    protected:

    private:

    /** Names of all of the behavior sets that this actor uses as input. */
    std::set<PluginDependencySet> myBehaviorSets;

    /** Name of the actor */
    std::string myName;
  };
}
