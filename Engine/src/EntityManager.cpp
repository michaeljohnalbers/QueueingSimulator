/**
 * @file EntityManager.cpp
 * @brief Definition of EntityManager class
 *
 * @author Michael Albers
 */

#include <algorithm>
#include "Actor.h"
#include "Behavior.h"
#include "BehaviorSet.h"
#include "EntityDependency.h"
#include "EntityManager.h"
#include "Plugin.h"
#include "PluginCollection.h"

QS::EntityManager::EntityManager(std::shared_ptr<PluginCollection> thePlugins) :
  myPlugins(thePlugins)
{
}

QS::EntityManager::~EntityManager()
{
  // Destory things from the top level down so no dependencies are pulled out
  // from underneath an object.

  for (auto actor : myActors)
  {
    actor.second->destroyActor(actor.first);
  }

  for (auto behaviorSet : myBehaviorSets)
  {
    behaviorSet.second->destroyBehaviorSet(behaviorSet.first);
  }

  for (auto behavior : myBehaviors)
  {
    behavior.second->destroyBehavior(behavior.first);
  }

  for (auto sensor : mySensors)
  {
    sensor.second->destroySensor(sensor.first);
  }
}

QS::Actor* QS::EntityManager::createActor(
  const std::string &theType,
  const PluginEntity::Properties &theProperties,
  const std::string &theSource)
{
  auto actorPlugin = myPlugins->getPlugin(theSource);

  auto actor = actorPlugin->createActor(theType, theProperties);
  myActors.push_back({actor, actorPlugin});

  auto actorPluginDefinition = actorPlugin->getDefinition();

  auto actorDefinitions = actorPluginDefinition->getActorDefinitions();
  auto actorDefinition =
    std::find_if(actorDefinitions.begin(),
                 actorDefinitions.end(),
                 [=](ActorDefinition &theDef) {
                   return (theDef.getName() == theType);
                 });

  std::vector<EntityDependency<BehaviorSet>> actorDependencies;
  auto neededBehaviorSets = actorDefinition->getBehaviorSets();
  for (DefinitionPair neededBehaviorSet : neededBehaviorSets)
  {
    EntityDependency<BehaviorSet> behaviorSetDependency {
      neededBehaviorSet.myName,
      createBehaviorSet(neededBehaviorSet.myName, {},
                        neededBehaviorSet.mySource),
      "" // TODO: need to implement tags
      };
    actorDependencies.push_back(behaviorSetDependency);
  }
  actor->setDependencies(actorDependencies);

  return actor;
}

QS::Behavior* QS::EntityManager::createBehavior(
  const std::string &theType,
  const PluginEntity::Properties &theProperties,
  const std::string &theSource)
{
  auto behaviorPlugin = myPlugins->getPlugin(theSource);

  auto behavior = behaviorPlugin->createBehavior(theType, theProperties);
  myBehaviors.push_back({behavior, behaviorPlugin});

  auto behaviorPluginDefinition = behaviorPlugin->getDefinition();

  auto behaviorDefinitions = behaviorPluginDefinition->getBehaviorDefinitions();
  auto behaviorDefinition =
    std::find_if(behaviorDefinitions.begin(),
                 behaviorDefinitions.end(),
                 [=](BehaviorDefinition &theDef) {
                   return (theDef.getName() == theType);
                 });

  std::vector<EntityDependency<Sensor>> behaviorDependencies;
  auto neededSensors = behaviorDefinition->getSensors();
  for (DefinitionPair neededSensor : neededSensors)
  {
    EntityDependency<Sensor> sensorDependency {
      neededSensor.myName,
      createSensor(neededSensor.myName, {}, neededSensor.mySource),
      "" // TODO: need to implement tags
      };
    behaviorDependencies.push_back(sensorDependency);
  }
  behavior->setDependencies(behaviorDependencies);

  return behavior;
}

QS::BehaviorSet* QS::EntityManager::createBehaviorSet(
  const std::string &theType,
  const PluginEntity::Properties &theProperties,
  const std::string &theSource)
{
  auto behaviorSetPlugin = myPlugins->getPlugin(theSource);

  auto behaviorSet = behaviorSetPlugin->createBehaviorSet(
    theType, theProperties);
  myBehaviorSets.push_back({behaviorSet, behaviorSetPlugin});

  auto behaviorSetPluginDefinition = behaviorSetPlugin->getDefinition();

  auto behaviorSetDefinitions =
    behaviorSetPluginDefinition->getBehaviorSetDefinitions();
  auto behaviorSetDefinition =
    std::find_if(behaviorSetDefinitions.begin(),
                 behaviorSetDefinitions.end(),
                 [=](BehaviorSetDefinition &theDef) {
                   return (theDef.getName() == theType);
                 });

  std::vector<EntityDependency<Behavior>> behaviorSetDependencies;
  auto neededBehaviors = behaviorSetDefinition->getBehaviors();
  for (DefinitionPair neededBehavior : neededBehaviors)
  {
    EntityDependency<Behavior> behaviorDependency {
      neededBehavior.myName,
      createBehavior(neededBehavior.myName, {}, neededBehavior.mySource),
      "" // TODO: need to implement tags
      };
    behaviorSetDependencies.push_back(behaviorDependency);
  }
  behaviorSet->setDependencies(behaviorSetDependencies);

  return behaviorSet;
}

QS::Sensor* QS::EntityManager::createSensor(
  const std::string &theType,
  const PluginEntity::Properties &theProperties,
  const std::string &theSource)
{
  auto sensorPlugin = myPlugins->getPlugin(theSource);

  Sensor *sensor = sensorPlugin->createSensor(theType, theProperties);

  mySensors.push_back({sensor, sensorPlugin});
  return sensor;
}
