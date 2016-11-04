#pragma once

/**
 * @file Plugin.h
 * @brief Encapsulates a single plugin at run-time.
 *
 * @author Michael Albers
 */

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "PluginDefinition.h"

namespace QS
{
  class Actor;
  class Behavior;
  class BehaviorSet;
  class Exit;
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
     * all types of T's that are defined in the plugin.
     */
    template<class T>
    using CreatorFunction = T*(*)(
      const std::string &theType,
      const std::map<std::string, std::string> &theProperties,
      const std::string &theTag);

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
     * @param theDefinition
     *           populated plugin definition
     * @throw std::runtime_error On problems loading the shared library.
     */
    Plugin(std::shared_ptr<PluginDefinition> theDefinition);

    /**
     * Destructor.
     */
    ~Plugin();

    /**
     * Creates an Actor of the given type with the given properties. The caller
     * owns the pointer, but must use destroyActor to destroy the returned
     * object.
     *
     * @param theType
     *          actor type name
     * @param theProperties
     *          key/value pairs of properties specific to the type of actor
     *          being created.
     * @param theTag
     *          optional user-defined tag for differentiating Actors of the
     *          same type
     * @throw std::invalid_argument On invalid actor type
     * @throw std::logic_error If no creator function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    Actor* createActor(const std::string &theType,
                       const std::map<std::string, std::string> &theProperties,
                       const std::string &theTag);

    /**
     * Creates the Behavior with the given name. The caller owns the pointer,
     * but must use destroyBehavior to destroy the returned object.
     *
     * @param theBehavior
     *          name of the behavior
     * @param theProperties
     *          key/value pairs of properties specific to the type of behavior
     *          being created.
     * @param theTag
     *          optional user-defined tag for differentiating Behaviors of the
     *          same type
     * @throw std::invalid_argument On invalid behavior name
     * @throw std::logic_error If no creator function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    Behavior* createBehavior(
      const std::string &theBehavior,
      const std::map<std::string, std::string> &theProperties,
      const std::string &theTag);

    /**
     * Creates the BehaviorSet with the given name. The caller owns the pointer,
     * but must use destroyBehavior to destroy the returned object.
     *
     * @param theBehaviorSet
     *          name of the behavior set
     * @param theProperties
     *          key/value pairs of properties specific to the type of behavior
     *          set being created.
     * @param theTag
     *          optional user-defined tag for differentiating BehaviorSets of
     *          the same type
     * @throw std::invalid_argument On invalid behavior name
     * @throw std::logic_error If no creator function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    BehaviorSet* createBehaviorSet(
      const std::string &theBehaviorSet,
      const std::map<std::string, std::string> &theProperties,
      const std::string &theTag);

    /**
     * Creates the Exit with the given name. The caller owns the pointer, but
     * must use destroyExit to destroy the returned object.
     *
     * @param theExit
     *          name of the exit
     * @param theProperties
     *          key/value pairs of properties specific to the type of exit
     *          being created.
     * @param theTag
     *          optional user-defined tag for differentiating Exits of the
     *          same type
     * @throw std::invalid_argument On invalid exit name
     * @throw std::logic_error If no creator function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    Exit* createExit(
      const std::string &theExit,
      const std::map<std::string, std::string> &theProperties,
      const std::string &theTag);

    /**
     * Creates the Sensor with the given name. The caller owns the pointer, but
     * must use destroySensor to destroy the returned object.
     *
     * @param theSensor
     *          name of the sensor
     * @param theProperties
     *          key/value pairs of properties specific to the type of sensor
     *          being created.
     * @param theTag
     *          optional user-defined tag for differentiating Sensors of the
     *          same type
     * @throw std::invalid_argument On invalid sensor name
     * @throw std::logic_error If no creator function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    Sensor* createSensor(
      const std::string &theSensor,
      const std::map<std::string, std::string> &theProperties,
      const std::string &theTag);

    /**
     * Destroys actors created from createActor.
     *
     * @param theActor
     *          actor to destroy
     * @throw std::logic_error If no destructor function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    void destroyActor(Actor *theActor) const;

    /**
     * Destroys the behavior from createBehavior.
     *
     * @param theBehavior
     *          behavior to destroy
     * @throw std::logic_error If no destructor function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    void destroyBehavior(Behavior *theBehavior) const;

    /**
     * Destroys the behavior set from createBehaviorSet.
     *
     * @param theBehaviorSet
     *          behavior set to destroy
     * @throw std::logic_error If no destructor function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    void destroyBehaviorSet(BehaviorSet *theBehaviorSet) const;

    /**
     * Destroys the exit from createExit.
     *
     * @param theExit
     *          exit to destroy
     * @throw std::logic_error If no destructor function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    void destroyExit(Exit *theExit) const;

    /**
     * Destroys the sensor from createSensor.
     *
     * @param theSensor
     *          sensor to destroy
     * @throw std::logic_error If no destructor function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    void destroySensor(Sensor *theSensor) const;

    /**
     * Returns the definition of this plugin.
     *
     * @return plugin definition
     */
    std::shared_ptr<PluginDefinition> getDefinition() const noexcept;

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

    /**
     * Generic creator function.
     *
     * @param theSensor
     *          type name of thing being created
     * @param theProperties
     *          key/value pairs of properties specific to the type of thing
     *          being created.
     * @param theTag
     *          optional user-defined tag for differentiating things of the
     *          same type
     * @param theCreatorDestructor
     *          creator/destructor functions
     * @param theName
     *          description of the thing being created
     * @throw std::invalid_argument On invalid type name
     * @throw std::logic_error If no creator function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    template<class T>
    T* create(const std::string &theType,
              const std::map<std::string, std::string> &theProperties,
              const std::string &theTag,
              CreatorDestructor<T> theCreatorDestructor,
              const std::string &theName);

    /**
     * Generic destruction funcction.
     *
     * @param theObject
     *          object to destroy
     * @param theCreatorDestructor
     *          creator/destructor functions
     * @param theName
     *          description of the thing being destroyed
     * @throw std::logic_error If no destructor function defined (this shouldn't
     *          happen and is indicative of an internal error, it might be
     *          best to ignore this exception and let it percolate up)
     */
    template<class T>
    void destroy(T *theObject,
                 CreatorDestructor<T> theCreatorDestructor,
                 const std::string &theName) const;

    /**
     * Generic function for loading creator/destructor functions.
     *
     * @param theCreatorDestructor
     *          name of creator and destructor functions
     * @param theType
     *          description of functions being loaded
     * @throw std::runtime_error
     *          on error loading functions
     */
    template<class T>
    CreatorDestructor<T> loadCreatorDestructor(
      PluginDefinition::CreatorDestructorPair theCreatorDestructor,
      const std::string &theType);

    /**
     * Opens the library.
     *
     * @throws std::runtime_error
     *            if library cannot be opened.
     */
    void openLibrary();

    /** Definition of the plugin. */
    std::shared_ptr<PluginDefinition> myDefinition;

    /** Creator/destructor functions for Actors. */
    CreatorDestructor<Actor> myActorCreatorDestructor{nullptr, nullptr};

    /** Creator/destructor functions for Behaviors. */
    CreatorDestructor<Behavior> myBehaviorCreatorDestructor{nullptr, nullptr};

    /** Creator/destructor functions for BehaviorSets. */
    CreatorDestructor<BehaviorSet> myBehaviorSetCreatorDestructor{
      nullptr, nullptr};

    /** Creator/destructor functions for Exits. */
    CreatorDestructor<Exit> myExitCreatorDestructor{nullptr, nullptr};

    /** Creator/destructor functions for Sensors. */
    CreatorDestructor<Sensor> mySensorCreatorDestructor{nullptr, nullptr};

    /** Library handle from dlopen. */
    void *myLibraryHandle;
  };
}
