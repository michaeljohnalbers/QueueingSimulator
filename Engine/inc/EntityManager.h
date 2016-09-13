#pragma once

/**
 * @file EntityManager.h
 * @brief Used to create Actors/Behaviors/etc. and own the created objects.
 *
 * @author Michael Albers
 */

#include <map>
#include <memory>
#include <tuple>
#include <string>
#include <vector>

namespace QS
{
  class Actor;
  class Behavior;
  class BehaviorSet;
  class Plugin;
  class PluginCollection;
  class Sensor;

  /**
   * This class provides one-stop shopping for creating all plugin entities
   * from any loaded plugin. When creating an entitiy all of its dependencies
   * (and the dependencies of those dependencies, etc.) are created too. In
   * other words, the returned entity object is fully populated and ready to
   * go.
   *
   * This class owns all created entities and will destroy them all upon
   * destruction.
   */
  class EntityManager
  {
    public:

    /**
     * Default constructor.
     */
    EntityManager() = delete;

    /**
     * Constructor.
     *
     * @param thePlugins
     *          all loaded plugins
     */
    EntityManager(std::shared_ptr<PluginCollection> thePlugins);

    /**
     * Copy constructor.
     */
    EntityManager(const EntityManager&) = delete;

    /**
     * Move constructor.
     */
    EntityManager(EntityManager&&) = delete;

    /**
     * Destructor.
     */
    ~EntityManager();

    /**
     * Creates the specified Actor.
     *
     * @param theType
     *          Actor type name, passed to plugin creator function
     * @param theProperties
     *          properties of the Actor
     * @param theSource
     *          name of the plugin where the Actor class is found
     * @throws std::invalid_argument
     *          if theType does not refer to a valid Actor type or
     *          theProperties contain invalid values
     *          theSource does not refer to a loaded plugin or
     */
    Actor* createActor(
      const std::string &theType,
      const PluginEntity::Properties &theProperties,
      const std::string &theSource);

    /**
     * Creates the specified Behavior.
     *
     * @param theType
     *          Behavior type name, passed to plugin creator function
     * @param theProperties
     *          properties of the Behavior
     * @param theSource
     *          name of the plugin where the Behavior class is found
     * @throws std::invalid_argument
     *          if theType does not refer to a valid Behavior type or
     *          theProperties contain invalid values
     *          theSource does not refer to a loaded plugin or
     */
    Behavior* createBehavior(
      const std::string &theType,
      const PluginEntity::Properties &theProperties,
      const std::string &theSource);

    /**
     * Creates the specified BehaviorSet.
     *
     * @param theType
     *          BehaviorSet type name, passed to plugin creator function
     * @param theProperties
     *          properties of the BehaviorSet
     * @param theSource
     *          name of the plugin where the BehaviorSet class is found
     * @throws std::invalid_argument
     *          if theType does not refer to a valid Behaviorset type or
     *          theProperties contain invalid values
     *          theSource does not refer to a loaded plugin or
     */
    BehaviorSet* createBehaviorSet(
      const std::string &theType,
      const PluginEntity::Properties &theProperties,
      const std::string &theSource);

    /**
     * Creates the specified Sensor.
     *
     * @param theType
     *          Sensor type name, passed to plugin creator function
     * @param theProperties
     *          properties of the Sensor
     * @param theSource
     *          name of the plugin where the Sensor class is found
     * @throws std::invalid_argument
     *          if theType does not refer to a valid Sensor type or
     *          theProperties contain invalid values
     *          theSource does not refer to a loaded plugin or
     */
    Sensor* createSensor(
      const std::string &theType,
      const PluginEntity::Properties &theProperties,
      const std::string &theSource);

    /**
     * Copy assignment operator
     */
    EntityManager& operator=(const EntityManager&) = delete;

    /**
     * Move assignment operator
     */
    EntityManager& operator=(EntityManager&&) = delete;

    protected:

    private:

    /** All created behaviors and the plugins which created them. */
    std::vector<std::pair<Actor*, std::shared_ptr<Plugin>>> myActors;

    /** All created behaviors and the plugins which created them. */
    std::vector<std::pair<Behavior*, std::shared_ptr<Plugin>>> myBehaviors;

    /** All created behaviors and the plugins which created them. */
    std::vector<std::pair<BehaviorSet*, std::shared_ptr<Plugin>>>
      myBehaviorSets;

    /** Collection of loaded plugins for creation/destruction of entities. */
    std::shared_ptr<PluginCollection> myPlugins;

    /** All created sensors and the plugins which created them. */
    std::vector<std::pair<Sensor*, std::shared_ptr<Plugin>>> mySensors;

  };
}
