/**
 * @file CreatorDestructor.cpp
 * @brief Contains all creator/destructor functions for the Basic Plugin
 *
 * @author Michael Albers
 */

#include <map>
#include <string>
#include "Actor.h"
#include "Walk.h"
#include "BasicWalk.h"
#include "NullSensor.h"

extern "C"
{
  QS::Actor* actorCreator(
    const std::string &theActorName,
    const QS::PluginEntity::Properties &theProperties)
  {
    // Only one type of Actor in this plugin, so just ignore the name.
    return new QS::Actor(theProperties);
  }

  void actorDestructor(QS::Actor *theActor)
  {
    delete theActor;
  }

  QS::BehaviorSet* behaviorSetCreator(
    const std::string &theBehaviorSetName,
    const QS::PluginEntity::Properties &theProperties)
  {
    // Only one behavior set in this plugin, so just ignore the name.
    return new QS::BasicWalk(theProperties);
  }

  void behaviorSetDestructor(QS::BehaviorSet *theBehaviorSet)
  {
    delete theBehaviorSet;
  }

  QS::Behavior* behaviorCreator(
    const std::string &theBehaviorName,
    const QS::PluginEntity::Properties &theProperties)
  {
    // Only one Behavior in this plugin, so ignore the name.
    return new QS::Walk(theProperties);
  }

  void behaviorDestructor(QS::Behavior *theBehavior)
  {
    delete theBehavior;
  }

  QS::Sensor* sensorCreator(
    const std::string &theSensorName,
    const QS::PluginEntity::Properties &theProperties)
  {
    // Only one sensor in this plugin, so ignore the name.
    return new QS::NullSensor(theProperties);
  }

  void sensorDestructor(QS::Sensor *theSensor)
  {
    delete theSensor;
  }
}

