/**
 * @file CreatorDestructor.cpp
 * @brief Contains all creator/destructor functions for the Queueing Plugin
 *
 * @author Michael Albers
 */

#include <string>
#include "ExitSeek.h"
#include "FindExitSensor.h"
#include "NearestN.h"
#include "LooseOrdering.h"
#include "OrderedActor.h"
#include "OrderedExit.h"

extern "C"
{
  QS::Actor* actorCreator(
    const std::string &theActorName,
    const QS::PluginEntity::Properties &theProperties,
    const std::string &theTag)
  {
    QS::Actor *actor = nullptr;
    if ("OrderedActor" == theActorName)
    {
      actor = new QS::OrderedActor(theProperties, theTag);
    }
    else
    {
      std::string error{"Invalid actor name given to QueueingPlugin Actor "};
      error += "creator, \"" + theActorName +
        "\". Valid type is \"OrderedActor\".";
      throw std::invalid_argument(error);
    }

    return actor;
  }

  void actorDestructor(QS::Actor *theActor)
  {
    delete theActor;
  }

  QS::BehaviorSet* behaviorSetCreator(
    const std::string &theBehaviorSetName,
    const QS::PluginEntity::Properties &theProperties,
    const std::string &theTag)
  {
    if ("LooseOrdering" == theBehaviorSetName)
    {
      return new QS::LooseOrdering(theProperties, "");
    }

    std::string error{"Invalid behavior set name given to QueueingPlugin "};
    error += "BehaviorSet creator, \"" + theBehaviorSetName +
      "\". Valid types are \"LooseOrdering\".";
    throw std::invalid_argument(error);
  }

  void behaviorSetDestructor(QS::BehaviorSet *theBehaviorSet)
  {
    delete theBehaviorSet;
  }

  QS::Behavior* behaviorCreator(
    const std::string &theBehaviorName,
    const QS::PluginEntity::Properties &theProperties,
    const std::string &theTag)
  {
    if ("ExitSeek" == theBehaviorName)
    {
      return new QS::ExitSeek(theProperties, "");
    }

    std::string error{"Invalid behavior name given to QueueingPlugin "};
    error += "Behavior creator, \"" + theBehaviorName +
      "\". Valid types are \"ExitSeek\".";
    throw std::invalid_argument(error);
  }

  void behaviorDestructor(QS::Behavior *theBehavior)
  {
    delete theBehavior;
  }

  QS::Sensor* sensorCreator(
    const std::string &theSensorName,
    const QS::PluginEntity::Properties &theProperties,
    const std::string &theTag)
  {
    if ("FindExitSensor" == theSensorName)
    {
      return new QS::FindExitSensor(theProperties, "");
    }
    else if ("NearestN" == theSensorName)
    {
      return new QS::NearestN(theProperties, "");
    }

    std::string error{"Invalid sensor name given to QueueingPlugin Sensor "};
    error += "creator, \"" + theSensorName +
      "\". Valid types are \"FindExitSensor\" and \"NearestN\".";
    throw std::invalid_argument(error);
  }

  void sensorDestructor(QS::Sensor *theSensor)
  {
    delete theSensor;
  }

  QS::Exit* exitCreator(
    const std::string &theExitName,
    const QS::PluginEntity::Properties &theProperties,
    const std::string &theTag)
  {
    // Only one exit in this plugin, so ignore the name.
    return new QS::OrderedExit(theProperties, theTag);
  }

  void exitDestructor(QS::Exit *theExit)
  {
    delete theExit;
  }
}
