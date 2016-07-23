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
#include "SensorDefinition.h"

class Actor;
class Behavior;
class Sensor;

namespace QS
{
  /**
   * Encapsulates all data for a single plugin. A plugin can define any subset
   * of the following: Actors, Behaviors and Sensors. Plugins allow
   * user-defined extensions to Queueing Simulator.
   */
  class Plugin
  {
    public:

    /**
     * Definition of a creator function. This function must be able to create
     * all types of T's, which do not use attributes, that are defined in the
     * plugin.
     */
    template<class T>
    using CreatorFunction = T*(*)(const std::string &theType);

    /**
     * Definition of a creator function. This function must be able to create
     * all types of T's, which use attributes, that are defined in the plugin.
     */
    template<class T>
    using CreatorAttributesFunction = T*(*)(
      const std::string &theType,
      const std::map<std::string, std::string> &theAttributes);

    /**
     * Definition of a destructor function. This function must be able to
     * destroy all types of T's defined in the plugin.
     */
    template<class T>
    using DestructorFunction = void(*)(T *theObject);

    /** Pair to hold the creator and destructor functions. */
    template <class T>
    using CreatorDestructor = std::pair<CreatorFunction<T>,
                                        DestructorFunction<T>>;

    /** Pair to hold the creator and destructor functions. */
    template <class T>
    using CreatorDestructorAttributes = std::pair<CreatorAttributesFunction<T>,
                                                  DestructorFunction<T>>;

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
    Plugin(const Plugin&) = delete;

    /**
     * Move constructor.
     */
    Plugin(Plugin&&) = delete;

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
     * Adds a new sensor definition to this plugin.
     *
     * @param theSensorDefinition
     *          sensor definition to add
     * @throws std::invalid_argument
     *           if another definition with the same type name already exists
     */
    void addSensorDefinition(const SensorDefinition &theSensorDefinition);

    /**
     * Returns the creator/destructor functions for Actors.
     *
     * @return creator/destructor functions
     */
    CreatorDestructorAttributes<Actor> getActorCreatorDestructor()
      const noexcept;

    /**
     * Returns the list of all Actors defined in this plugin.
     *
     * @return custom actors
     */
    std::vector<ActorDefinition> getActorDefinitions() const noexcept;

    /**
     * Returns the creator/destructor functions for Behaviors.
     *
     * @return creator/destructor functions
     */
    CreatorDestructorAttributes<Behavior> getBehaviorCreatorDestructor()
      const noexcept;

    /**
     * Returns the list of all Behaviors defined in this plugin.
     *
     * @return custom behaviors
     */
    std::vector<BehaviorDefinition> getBehaviorDefinitions() const noexcept;

    /**
     * Returns the creator/destructor functions for Sensors.
     *
     * @return creator/destructor functions
     */
    CreatorDestructor<Sensor> getSensorCreatorDestructor() const noexcept;

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
    Plugin& operator=(const Plugin&) = delete;

    /**
     * Move assignment operator.
     */
    Plugin& operator=(Plugin&&) = delete;

    /**
     * Sets the creator/destructor functions for Actors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor
     */
    void setActorCreatorDestructor(CreatorFunction<Actor> theCreator,
                                   DestructorFunction<Actor> theDestructor);

    /**
     * Sets the creator/destructor functions for Behaviors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor
     */
    void setBehaviorCreatorDestructor(
      CreatorFunction<Behavior> theCreator,
      DestructorFunction<Behavior> theDestructor);

    /**
     * Sets the creator/destructor functions for Sensors.
     *
     * @param theCreator
     *          creator function
     * @param theDestructor
     *          destructor
     */
    void setSensorCreatorDestructor(
      CreatorFunction<Sensor> theCreator,
      DestructorFunction<Sensor> theDestructor);

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
    CreatorDestructorAttributes<Actor> myActorCreatorDestructor;

    /** All types of behaviors defined in this plugin. */
    std::vector<BehaviorDefinition> myBehaviors;

    /** Creator/destructor functions for Behaviors. */
    CreatorDestructorAttributes<Behavior> myBehaviorCreatorDestructor;

    /** All types of sensors defined in this plugin. */
    std::vector<SensorDefinition> mySensors;

    /** Creator/destructor functions for Sensors. */
    CreatorDestructor<Sensor> mySensorCreatorDestructor;

    /** Library implementing the plugin. */
    std::string myLibrary;

    /** Name of the plugin. */
    const std::string myName;
  };
}
