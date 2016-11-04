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
#include "SimulationEntityConfiguration.h"

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

  for (auto exit : myExits)
  {
    exit.second->destroyExit(exit.first);
  }
}

QS::Actor* QS::EntityManager::createActor(
  const SimulationEntityConfiguration &theActorConfiguration)
{
  auto actorType = theActorConfiguration.getType();

  auto actorPlugin = myPlugins->getPlugin(theActorConfiguration.getSource());
  auto actor = actorPlugin->createActor(
    actorType,
    theActorConfiguration.getProperties(),
    theActorConfiguration.getTag());
  myActors.push_back({actor, actorPlugin});

  auto actorPluginDefinition = actorPlugin->getDefinition();

  auto actorDefinitions = actorPluginDefinition->getActorDefinitions();
  auto actorDefinition = std::find_if(actorDefinitions.begin(),
                                      actorDefinitions.end(),
                                      [=](ActorDefinition &theDef) {
                                        return (theDef.getName() == actorType);
                                      });

  std::vector<SimulationEntityConfiguration> actorDependencyConfigurations =
    theActorConfiguration.getDependencyConfigurations();

  std::vector<EntityDependency<BehaviorSet>> actorDependencies;
  auto neededBehaviorSets = actorDefinition->getBehaviorSets();
  for (PluginDependencySet neededBehaviorSet : neededBehaviorSets)
  {
    auto dependencyConfigurationIter =
      std::find_if(actorDependencyConfigurations.begin(),
                   actorDependencyConfigurations.end(),
                   [=](SimulationEntityConfiguration &theConfig){
                     return (theConfig.getType() == neededBehaviorSet.myName &&
                             theConfig.getTag() == neededBehaviorSet.myTag);
                   });

    // Didn't find a match with the tag, try for one without a tag.
    if (dependencyConfigurationIter == actorDependencyConfigurations.end())
    {
      dependencyConfigurationIter =
        std::find_if(actorDependencyConfigurations.begin(),
                     actorDependencyConfigurations.end(),
                     [=](SimulationEntityConfiguration &theConfig){
                       return (theConfig.getType() == neededBehaviorSet.myName);
                     });
    }

    // Start with a basic configuration that works if no configuration was
    // found. If no configuration found for this BehaviorSet (this also means
    // that there is no configuration for the BehaviorSet's dependencies) this
    // will contain enough information to create the BehaviorSet.
    SimulationEntityConfiguration dependencyConfiguration(
      neededBehaviorSet.myName,
      neededBehaviorSet.myTag,
      neededBehaviorSet.mySource);

    if (dependencyConfigurationIter != actorDependencyConfigurations.end())
    {
      dependencyConfiguration = *dependencyConfigurationIter;
    }

    EntityDependency<BehaviorSet> behaviorSetDependency {
      neededBehaviorSet.myName,
      createBehaviorSet(dependencyConfiguration),
      neededBehaviorSet.myTag};

    actorDependencies.push_back(behaviorSetDependency);
  }
  actor->setDependencies(actorDependencies);
  return actor;
}

QS::Behavior* QS::EntityManager::createBehavior(
  const SimulationEntityConfiguration &theBehaviorConfiguration)
{
  auto behaviorPlugin = myPlugins->getPlugin(
    theBehaviorConfiguration.getSource());

  auto type = theBehaviorConfiguration.getType();

  auto behavior = behaviorPlugin->createBehavior(
    type,
    theBehaviorConfiguration.getProperties(),
    theBehaviorConfiguration.getTag());
  myBehaviors.push_back({behavior, behaviorPlugin});

  auto behaviorPluginDefinition = behaviorPlugin->getDefinition();

  auto behaviorDefinitions =
    behaviorPluginDefinition->getBehaviorDefinitions();
  auto behaviorDefinition = std::find_if(behaviorDefinitions.begin(),
                                         behaviorDefinitions.end(),
                                         [=](BehaviorDefinition &theDef) {
                                           return (theDef.getName() == type);
                                         });

  std::vector<SimulationEntityConfiguration> sensorDependencyConfigurations =
    theBehaviorConfiguration.getDependencyConfigurations();

  std::vector<EntityDependency<Sensor>> behaviorDependencies;
  auto neededSensors = behaviorDefinition->getSensors();
  for (PluginDependencySet neededSensor : neededSensors)
  {
    auto dependencyConfigurationIter =
      std::find_if(sensorDependencyConfigurations.begin(),
                   sensorDependencyConfigurations.end(),
                   [=](SimulationEntityConfiguration &theConfig){
                     return (theConfig.getType() == neededSensor.myName &&
                             theConfig.getTag() == neededSensor.myTag);
                   });

    // Didn't find a match with the tag, try for one without a tag.
    if (dependencyConfigurationIter ==
        sensorDependencyConfigurations.end())
    {
      dependencyConfigurationIter =
        std::find_if(sensorDependencyConfigurations.begin(),
                     sensorDependencyConfigurations.end(),
                     [=](SimulationEntityConfiguration &theConfig){
                       return (theConfig.getType() == neededSensor.myName);
                     });
    }

    // Start with a basic configuration that works if no configuration was
    // found. If no configuration found for this Sensor this will contain
    // enough information to create the Sensor.
    SimulationEntityConfiguration dependencyConfiguration(
      neededSensor.myName, neededSensor.myTag, neededSensor.mySource);

    if (dependencyConfigurationIter !=
        sensorDependencyConfigurations.end())
    {
      dependencyConfiguration = *dependencyConfigurationIter;
    }

    EntityDependency<Sensor> sensorDependency {
      neededSensor.myName,
      createSensor(dependencyConfiguration),
      neededSensor.myTag};

    behaviorDependencies.push_back(sensorDependency);
  }
  behavior->setDependencies(behaviorDependencies);

  return behavior;
}


QS::BehaviorSet* QS::EntityManager::createBehaviorSet(
  const SimulationEntityConfiguration &theBehaviorSetConfiguration)
{
  auto behaviorSetPlugin = myPlugins->getPlugin(
    theBehaviorSetConfiguration.getSource());

  auto type = theBehaviorSetConfiguration.getType();

  auto behaviorSet = behaviorSetPlugin->createBehaviorSet(
    type, theBehaviorSetConfiguration.getProperties(),
    theBehaviorSetConfiguration.getTag());
  myBehaviorSets.push_back({behaviorSet, behaviorSetPlugin});

  auto behaviorSetPluginDefinition = behaviorSetPlugin->getDefinition();

  auto behaviorSetDefinitions =
    behaviorSetPluginDefinition->getBehaviorSetDefinitions();
  auto behaviorSetDefinition =
    std::find_if(behaviorSetDefinitions.begin(),
                 behaviorSetDefinitions.end(),
                 [=](BehaviorSetDefinition &theDef) {
                   return (theDef.getName() == type);
                 });

  std::vector<SimulationEntityConfiguration>
    behaviorSetDependencyConfigurations =
    theBehaviorSetConfiguration.getDependencyConfigurations();

  std::vector<EntityDependency<Behavior>> behaviorSetDependencies;
  auto neededBehaviors = behaviorSetDefinition->getBehaviors();
  for (PluginDependencySet neededBehavior : neededBehaviors)
  {

    auto dependencyConfigurationIter =
      std::find_if(behaviorSetDependencyConfigurations.begin(),
                   behaviorSetDependencyConfigurations.end(),
                   [=](SimulationEntityConfiguration &theConfig){
                     return (theConfig.getType() == neededBehavior.myName &&
                             theConfig.getTag() == neededBehavior.myTag);
                   });

    // Didn't find a match with the tag, try for one without a tag.
    if (dependencyConfigurationIter ==
        behaviorSetDependencyConfigurations.end())
    {
      dependencyConfigurationIter =
        std::find_if(behaviorSetDependencyConfigurations.begin(),
                     behaviorSetDependencyConfigurations.end(),
                     [=](SimulationEntityConfiguration &theConfig){
                       return (theConfig.getType() == neededBehavior.myName);
                     });
    }

    // Start with a basic configuration that works if no configuration was
    // found. If no configuration found for this Behavior (this also means that
    // there is no configuration for the Behavior's dependencies) this will
    // contain enough information to create the Behavior.
    SimulationEntityConfiguration dependencyConfiguration(
      neededBehavior.myName, neededBehavior.myTag, neededBehavior.mySource);

    if (dependencyConfigurationIter !=
        behaviorSetDependencyConfigurations.end())
    {
      dependencyConfiguration = *dependencyConfigurationIter;
    }

    EntityDependency<Behavior> behaviorDependency {
      neededBehavior.myName,
      createBehavior(dependencyConfiguration),
      neededBehavior.myTag};

    behaviorSetDependencies.push_back(behaviorDependency);
  }
  behaviorSet->setDependencies(behaviorSetDependencies);

  return behaviorSet;
}

QS::Exit* QS::EntityManager::createExit(
  const SimulationEntityConfiguration &theExitConfiguration)
{
  auto exitPlugin = myPlugins->getPlugin(theExitConfiguration.getSource());

  Exit *exit = exitPlugin->createExit(theExitConfiguration.getType(),
                                      theExitConfiguration.getProperties(),
                                      theExitConfiguration.getTag());

  myExits.push_back({exit, exitPlugin});

  return exit;
}

QS::Sensor* QS::EntityManager::createSensor(
  const SimulationEntityConfiguration &theSensorConfiguration)
{
  auto sensorPlugin = myPlugins->getPlugin(theSensorConfiguration.getSource());

  Sensor *sensor = sensorPlugin->createSensor(
    theSensorConfiguration.getType(),
    theSensorConfiguration.getProperties(),
    theSensorConfiguration.getTag());

  mySensors.push_back({sensor, sensorPlugin});

  return sensor;
}
