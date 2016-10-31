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

#include "PluginEntity.h"

namespace QS
{
  class Actor;
  class Behavior;
  class BehaviorSet;
  class Plugin;
  class PluginCollection;
  class SimulationEntityConfiguration;
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
     * @param theActorConfiguration
     *          configuration of the Actor and its dependencies
     * @throws std::invalid_argument
     *          if, in the configuration data, type does not refer to a valid
     *          Actor type or the properties contain invalid values
     *          the source does not refer to a loaded plugin or
     */
    Actor* createActor(
      const SimulationEntityConfiguration &theActorConfiguration);

    /**
     * Creates the specified Behavior.
     *
     * @param theBehaviorConfiguration
     *          configuration of the Behavior and its dependencies
     * @throws std::invalid_argument
     *          if, in the configuration data, type does not refer to a valid
     *          Behavior type or the properties contain invalid values
     *          the source does not refer to a loaded plugin or
     */
    Behavior* createBehavior(
      const SimulationEntityConfiguration &theBehaviorConfiguration);

    /**
     * Creates the specified BehaviorSet.
     *
     * @param theBehaviorSetConfiguration
     *          configuration of the BehaviorSet and its dependencies
     * @throws std::invalid_argument
     *          if, in the configuration data, type does not refer to a valid
     *          BehaviorSet type or the properties contain invalid values
     *          the source does not refer to a loaded plugin or
     */
    BehaviorSet* createBehaviorSet(
      const SimulationEntityConfiguration &theBehaviorSetConfiguration);

    /**
     * Creates the specified Sensor.
     *
     * @param theSensorConfiguration
     *          configuration of the Sensor
     * @throws std::invalid_argument
     *          if, in the configuration data, type does not refer to a valid
     *          Sensor type or the properties contain invalid values
     *          the source does not refer to a loaded plugin or
     */
    Sensor* createSensor(
      const SimulationEntityConfiguration &theSensorConfiguration);

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
