/**
 * @file CreatorDestructor.cpp
 * @brief Contains all creator/destructor functions for the Basic Plugin
 *
 * @author Michael Albers
 */

#include <map>
#include <string>
#include "Actor.h"
#include "Behavior.h"
#include "BehaviorSet.h"
#include "Sensor.h"

extern "C"
{
  QS::Actor* actorCreator(
    const std::string &theActorName,
    const std::map<std::string, std::string> &theProperties)
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
    const std::map<std::string, QS::Behavior*> &theBehaviors)
  {
    // Only one behavior set in this plugin, so just ignore the name.
    return new QS::BehaviorSet(theBehaviors);
  }

  void behaviorSetDestructor(QS::BehaviorSet *theBehaviorSet)
  {
    delete theBehaviorSet;
  }

  QS::Behavior* behaviorCreator(
    const std::string &theBehaviorName,
    const std::vector<std::string> &theInputSensorTypes)
  {
    // TODO: until an actual behavior is implemented this will have to do.
    return nullptr;
  }

  void behaviorDestructor(QS::Behavior *theBehavior)
  {
    delete theBehavior;
  }

  QS::Sensor* sensorCreator(const std::string &theSensorName)
  {
    // TODO: until an actual sensor is implemented, this will have to do.
    return nullptr;
  }

  void sensorDestructor(QS::Sensor *theSensor)
  {
    delete theSensor;
  }
}

