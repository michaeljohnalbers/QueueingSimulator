/**
 * @file CreatorDestructor.cpp
 * @brief Contains all creator/destructor functions for the Queueing Plugin
 *
 * @author Michael Albers
 */

#include <string>
#include "CollisionAvoidance.h"
#include "ExitFlee.h"
#include "ExitSeek.h"
#include "FindExitSensor.h"
#include "GreedyOrderedActor.h"
#include "GreedyOrdering.h"
#include "NearestN.h"
#include "NearExitArrival.h"
#include "LooseOrderedActor.h"
#include "LooseOrdering.h"
#include "OrderedActor.h"
#include "OrderedExit.h"
#include "OrderedLeaderFollow.h"
#include "SemiRationalOrdering.h"
#include "Separation.h"

extern "C"
{
  QS::Actor* actorCreator(
    const std::string &theActorName,
    const QS::PluginEntity::Properties &theProperties,
    const std::string &theTag)
  {
    QS::Actor *actor = nullptr;
    if ("GreedyOrderedActor" == theActorName)
    {
      actor = new QS::GreedyOrderedActor(theProperties, theTag);
    }
    else if ("OrderedActor" == theActorName)
    {
      actor = new QS::OrderedActor(theProperties, theTag);
    }
    else if ("LooseOrderedActor" == theActorName)
    {
      actor = new QS::LooseOrderedActor(theProperties, theTag);
    }
    else
    {
      std::string error{"Invalid actor name given to QueueingPlugin Actor "};
      error += "creator, \"" + theActorName +
        "\". Valid types are \"GreedyOrderedActor\", \"LooseOrderedActor\" " +
        "and \"OrderedActor\".";
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
    if ("GreedyOrdering" == theBehaviorSetName)
    {
      return new QS::GreedyOrdering(theProperties, "");
    }
    else if ("LooseOrdering" == theBehaviorSetName)
    {
      return new QS::LooseOrdering(theProperties, "");
    }
    else if ("SemiRationalOrdering" == theBehaviorSetName)
    {
      return new QS::SemiRationalOrdering(theProperties, "");
    }

    std::string error{"Invalid behavior set name given to QueueingPlugin "};
    error += "BehaviorSet creator, \"" + theBehaviorSetName +
      "\". Valid types are \"GreedyOrdering\", \"LooseOrdering\" and" +
      "\"SemiRationalOrdering\". ";
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
    if ("CollisionAvoidance" == theBehaviorName)
    {
      return new QS::CollisionAvoidance(theProperties, "");
    }
    else if ("ExitFlee" == theBehaviorName)
    {
      return new QS::ExitFlee(theProperties, "");
    }
    else if ("ExitSeek" == theBehaviorName)
    {
      return new QS::ExitSeek(theProperties, "");
    }
    else if ("NearExitArrival" == theBehaviorName)
    {
      return new QS::NearExitArrival(theProperties, "");
    }
    else if ("OrderedLeaderFollow" == theBehaviorName)
    {
      return new QS::OrderedLeaderFollow(theProperties, "");
    }
    else if ("Separation" == theBehaviorName)
    {
      return new QS::Separation(theProperties, "");
    }

    std::string error{"Invalid behavior name given to QueueingPlugin "};
    error += "Behavior creator, \"" + theBehaviorName +
      "\". Valid types are \"ExitFlee\", \"ExitSeek\", " +
      "\"CollisionAvoidance\", \"NearExitArrival\", " +
      "\"OrderedLeaderFollow\" and \"Separation\".";
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
