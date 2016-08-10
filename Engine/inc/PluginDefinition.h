#pragma once

/**
 * @file Plugin.h
 * @brief Contains all the information for a single plugin.
 *
 * @author Michael Albers
 */

#include <string>
#include <utility>

#include "ActorDefinition.h"
#include "BehaviorDefinition.h"
#include "BehaviorSetDefinition.h"
#include "SensorDefinition.h"

class Actor;
class Behavior;
class Sensor;

namespace QS
{
  /**
   * Encapsulates all data for a single plugin. A plugin can define any subset
   * of the following: Actors, Behaviors, BehaviorSets and Sensors. Plugins
   * allow user-defined extensions to Queueing Simulator.
   */
  class Plugin
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
    Plugin() = delete;

    /**
     * Constructor.
     *
     * @param theName
     *          plugin's name
     */
    Plugin(const std::string &theName);

    /**
     * Copy constructor.
     */
    Plugin(const Plugin&) = default;

    /**
     * Move constructor.
     */
    Plugin(Plugin&&) = default;

    /**
     * Destructor.
     */
    ~Plugin() = default;

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
      const BehaviorSetDefinition &theBehaviorDefinition);

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
     * Returns the creator/destructor function namess for BehaviorSets.
     *
     * @return creator/destructor functions
     */
    CreatorDestructorPair getBehaviorSetCreatorDestructor() const noexcept;

    /**
     * Returns the list of all Behaviors defined in this plugin.
     *
     * @return custom behaviors
     */
    std::vector<BehaviorDefinition> getBehaviorDefinitions() const noexcept;

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
     * Copy assignment operator.
     */
    Plugin& operator=(const Plugin&) = default;

    /**
     * Move assignment operator.
     */
    Plugin& operator=(Plugin&&) = default;

    /**
     * Sets the creator/destructor function names for Actors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor function
     */
    void setActorCreatorDestructor(const std::string &theCreator,
                                   const std::string &theDestructor);

    /**
     * Sets the creator/destructor function names for Behaviors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor function
     */
    void setBehaviorCreatorDestructor(const std::string &theCreator,
                                      const std::string & theDestructor);

    /**
     * Sets the creator/destructor function names for BehaviorSets.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor function
     */
    void setBehaviorSetCreatorDestructor(const std::string &theCreator,
                                         const std::string & theDestructor);

    /**
     * Sets the creator/destructor function names for Sensors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor function
     */
    void setSensorCreatorDestructor(const std::string &theCreator,
                                    const std::string &theDestructor);

    /**
     * Sets the name of the implementing library.
     *
     * @param theLibrary
     *          plugin library name
     */
    void setLibrary(const std::string &theLibrary) noexcept;

    protected:

    private:

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

    /** All types of sensors defined in this plugin. */
    std::vector<SensorDefinition> mySensors;

    /** Creator/destructor functions for Sensors. */
    CreatorDestructorPair mySensorCreatorDestructor;

    /** Library implementing the plugin. */
    std::string myLibrary;

    /** Name of the plugin. */
    const std::string myName;
  };
}