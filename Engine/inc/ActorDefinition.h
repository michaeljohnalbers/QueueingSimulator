#pragma once

/**
 * @file ActorDefinition.h
 * @brief Actor definition as defined in the plugin file.
 *
 * @author Michael Albers
 */

#include <string>
#include <vector>

namespace QS
{
  /**
   * Contains the definition for one type of actor as defined in a plugin
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
     *          specific name of Actor type
     * @param theBehaviors
     *          names of input behaviors to this actor
     */
    ActorDefinition(const std::string &theName,
                    const std::vector<std::string> &theBehaviors);

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
     * Returns the list of behavior sets this actor uses.
     *
     * @return behavior set list
     */
    std::vector<std::string> getBehaviorSets() const noexcept;

    /**
     * Returns the name of the type of Actor.
     *
     * @return actor type name
     */
    std::string getName() const noexcept;

    /**
     * Equality operator.
     *
     * @param theOther
     *          actor definition to compare against
     * @return true if the type names are equal, false otherwise
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
    const std::vector<std::string> myBehaviorSets;

    /** Name of the type of actor */
    const std::string myName;
  };
}

