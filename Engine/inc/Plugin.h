#pragma once

/**
 * @file Plugin.h
 * @brief Encapsulates a single plugin at run-time.
 *
 * @author Michael Albers
 */

#include <map>
#include <string>
#include <utility>
#include "PluginDefinition.h"

namespace QS
{
  class Actor;
  class Behavior;
  class Sensor;

  /**
   * Run-time interface for a single plugin. This function manages loading the
   * plugin's shared object library and provides functions for creation and
   * destruction of the pieces of the plugin.
   */
  class Plugin
  {
    public:

    /**
     * Definition of a creator function. This function must be able to create
     * all types of T's, which do not use properties, that are defined in the
     * plugin.
     */
    template<class T>
    using CreatorFunction = T*(*)(const std::string &theType);

    /**
     * Definition of a creator function. This function must be able to create
     * all types of T's, which use properties, that are defined in the plugin.
     */
    template<class T>
    using CreatorPropertiesFunction = T*(*)(
      const std::string &theType,
      const std::map<std::string, std::string> &theProperties);

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
    using CreatorDestructorProperties = std::pair<CreatorPropertiesFunction<T>,
                                                  DestructorFunction<T>>;

    /**
     * Default constructor.
     */
    Plugin() = delete;

    /**
     * Copy constructor.
     */
    Plugin(const Plugin&) = delete;

    /**
     * Move constructor.
     */
    Plugin(Plugin&&) = delete;

    /**
     * Constructor. Constructs the plugin from the given definition. The shared
     * library is also loaded here.
     *
     * @throw std::runtime_error On problems loading the shared library.
     */
    Plugin(const PluginDefinition &theDefinition);

    /**
     * Destructor.
     */
    ~Plugin();

    /**
     * Creates an actor of the given type with the given properties. The caller
     * owns the pointer, but must use destroyActor to destroy the returned
     * object.
     *
     * @param theType
     *          actor type name
     * @param theProperties
     *          key/value pairs of properties specific to the type of actor
     *          being created.
     * @throw std::invalid_argument On invalid actor type
     */
    Actor* createActor(const std::string &theType,
                       const std::map<std::string, std::string> &theProperties);

    /**
     * Creates the behavior with the given name. The caller owns the pointer,
     * but must use destroyBehavior to destroy the returned object.
     *
     * @param theBehavior
     *          name of the behavior
     * @throw std::invalid_argument On invalid behavior name
     */
    Behavior* createBehavior(const std::string &theBehavior);

    /**
     * Creates the sensor with the given name. The caller owns the pointer, but
     * must use destroySensor to destroy the returned object.
     *
     * @param theSensor
     *          name of the sensor
     * @throw std::invalid_argument On invalid sensor name
     */
    Sensor* createSensor(const std::string &theSensor);

    /**
     * Destroys actors created from createActor.
     *
     * @param theActor
     *          actor to destroy
     */
    void destroyActor(Actor *theActor) const;

    /**
     * Destroys the behavior from createBehavior.
     *
     * @param theBehavior
     *          behavior to destroy
     */
    void destroyBehavior(Behavior *theBehavior) const;

    /**
     * Destroys the sensor from createSensor.
     *
     * @param theSensor
     *          sensor to destroy
     */
    void destroySensor(Sensor *theSensor) const;

    /**
     * Returns the plugin name.
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

    protected:

    private:

    /** Definition of the plugin. */
    const PluginDefinition myDefinition;

    /** Creator/destructor functions for Actors. */
    CreatorDestructorProperties<Actor> myActorCreatorDestructor;

    /** Creator/destructor functions for Behaviors. */
    CreatorDestructorProperties<Behavior> myBehaviorCreatorDestructor;

    /** Creator/destructor functions for Sensors. */
    CreatorDestructor<Sensor> mySensorCreatorDestructor;

    /** Library handle from dlopen. */
    void *myLibraryHandle;
  };
}
