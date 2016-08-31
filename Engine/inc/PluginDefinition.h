#pragma once

/**
 * @file PluginDefinition.h
 * @brief Contains all the information for a single plugin from its definition
 *        file.
 *
 * @author Michael Albers
 */

#include <string>
#include <utility>
#include <vector>

#include "ActorDefinition.h"
#include "BehaviorDefinition.h"
#include "BehaviorSetDefinition.h"
#include "SensorDefinition.h"

namespace QS
{
  /**
   * Encapsulates all data for a single plugin. A plugin can define any subset
   * of the following: Actors, Behaviors, BehaviorSets and Sensors. Plugins
   * allow user-defined extensions to Queueing Simulator.
   */
  class PluginDefinition
  {
    public:

    /**
     * Type definition for coupling the function names for creation and
     * deletion. First name is the constructor, second is the destructor.
     */
    using CreatorDestructorPair = std::pair<std::string, std::string>;

    /**
     * Default constructor.
     */
    PluginDefinition() = delete;

    /**
     * Constructor.
     *
     * @param theDirectory
     *          directory where plugin files are located
     */
    PluginDefinition(const std::string &theDirectory);

    /**
     * Copy constructor.
     */
    PluginDefinition(const PluginDefinition&) = default;

    /**
     * Move constructor.
     */
    PluginDefinition(PluginDefinition&&) = default;

    /**
     * Destructor.
     */
    ~PluginDefinition() = default;

    /**
     * Adds a new actor definition to this plugin.
     *
     * @param theActorDefinition
     *          actor definition to add
     * @throws std::invalid_argument
     *           if another definition with the same type name already exists
     */
    void addActorDefinition(const ActorDefinition &theActorDefinition);

    /**
     * Adds a new behavior definition to this plugin.
     *
     * @param theBehaviorDefinition
     *          behavior definition to add
     * @throws std::invalid_argument
     *           if another definition with the same type name already exists
     */
    void addBehaviorDefinition(const BehaviorDefinition &theBehaviorDefinition);

    /**
     * Adds a new behavior set definition to this plugin.
     *
     * @param theBehaviorSetDefinition
     *          behavior set definition to add
     * @throws std::invalid_argument
     *           if another definition with the same name already exists
     */
    void addBehaviorSetDefinition(
      const BehaviorSetDefinition &theBehaviorSetDefinition);

    /**
     * Adds a new sensor definition to this plugin.
     *
     * @param theSensorDefinition
     *          sensor definition to add
     * @throws std::invalid_argument
     *           if another definition with the same type name already exists
     */
    void addSensorDefinition(const SensorDefinition &theSensorDefinition);

    /**
     * Returns the creator/destructor function names for Actors.
     *
     * @return creator/destructor functions
     */
    CreatorDestructorPair getActorCreatorDestructor() const noexcept;

    /**
     * Returns the list of all Actors defined in this plugin.
     *
     * @return custom actors
     */
    std::vector<ActorDefinition> getActorDefinitions() const noexcept;

    /**
     * Returns the creator/destructor function namess for Behaviors.
     *
     * @return creator/destructor functions
     */
    CreatorDestructorPair getBehaviorCreatorDestructor() const noexcept;

    /**
     * Returns the list of all Behaviors defined in this plugin.
     *
     * @return custom behaviors
     */
    std::vector<BehaviorDefinition> getBehaviorDefinitions() const noexcept;

    /**
     * Returns the creator/destructor function namess for BehaviorSets.
     *
     * @return creator/destructor functions
     */
    CreatorDestructorPair getBehaviorSetCreatorDestructor() const noexcept;

    /**
     * Returns the list of all Behavior sets defined in this plugin.
     *
     * @return custom behavior sets
     */
    std::vector<BehaviorSetDefinition> getBehaviorSetDefinitions()
      const noexcept;

    /**
     * Returns the directory containing the plugin files.
     *
     * @return plugin directory
     */
    std::string getDirectory() const noexcept;

    /**
     * Returns the name of the library which implements this plugin.
     *
     * @return library name
     */
    std::string getLibrary() const noexcept;

    /**
     * Returns the name of this plugin.
     *
     * @return plugin name
     */
    std::string getName() const noexcept;

    /**
     * Returns the creator/destructor function names for Sensors.
     *
     * @return creator/destructor functions
     */
    CreatorDestructorPair getSensorCreatorDestructor() const noexcept;

    /**
     * Returns the list of all Sensors defined in this plugin.
     *
     * @return custom sensors
     */
    std::vector<SensorDefinition> getSensorDefinitions() const noexcept;

    /**
     * Copy assignment operator.
     */
    PluginDefinition& operator=(const PluginDefinition&) = default;

    /**
     * Move assignment operator.
     */
    PluginDefinition& operator=(PluginDefinition&&) = default;

    /**
     * Sets the creator/destructor function names for Actors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor function
     */
    void setActorCreatorDestructor(const std::string &theCreator,
                                   const std::string &theDestructor) noexcept;

    /**
     * Sets the creator/destructor function names for Behaviors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor function
     */
    void setBehaviorCreatorDestructor(const std::string &theCreator,
                                      const std::string &theDestructor)
      noexcept;

    /**
     * Sets the creator/destructor function names for BehaviorSets.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor function
     */
    void setBehaviorSetCreatorDestructor(const std::string &theCreator,
                                         const std::string &theDestructor)
      noexcept;

    /**
     * Sets the name of the implementing library.
     *
     * @param theLibrary
     *          plugin library name
     */
    void setLibrary(const std::string &theLibrary) noexcept;

    /**
     * Sets the plugin's name from the definition file.
     *
     * @param theName
     *          plugin's name
     */
    void setName(const std::string &theName) noexcept;

    /**
     * Sets the creator/destructor function names for Sensors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor function
     */
    void setSensorCreatorDestructor(const std::string &theCreator,
                                    const std::string &theDestructor)
      noexcept;

    protected:

    private:

    /**
     * Helper function for the add* functions.
     */
    template <class T>
    void addItem(std::vector<T> &theVector, const T &theItem,
                 const std::string &theDescription);

    /** All types of actors defined in this plugin. */
    std::vector<ActorDefinition> myActors;

    /** Creator/destructor functions for Actors. */
    CreatorDestructorPair myActorCreatorDestructor;

    /** All types of behaviors defined in this plugin. */
    std::vector<BehaviorDefinition> myBehaviors;

    /** Creator/destructor functions for Behaviors. */
    CreatorDestructorPair myBehaviorCreatorDestructor;

    /** All types of behavior sets defined in this plugin. */
    std::vector<BehaviorSetDefinition> myBehaviorSets;

    /** Creator/destructor functions for BehaviorSets. */
    CreatorDestructorPair myBehaviorSetCreatorDestructor;

    /** Directory containing plugin files. */
    std::string myDirectory;

    /** All types of sensors defined in this plugin. */
    std::vector<SensorDefinition> mySensors;

    /** Creator/destructor functions for Sensors. */
    CreatorDestructorPair mySensorCreatorDestructor;

    /** Library implementing the plugin. */
    std::string myLibrary;

    /** Name of the plugin. */
    std::string myName;
  };
}
