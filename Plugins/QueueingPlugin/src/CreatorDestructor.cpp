/**
 * @file CreatorDestructor.cpp
 * @brief Contains all creator/destructor functions for the Queueing Plugin
 *
 * @author Michael Albers
 */

#include <string>
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
    if (strcasecmp(theActorName.c_str(), "OrderedActor") == 0)
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

  // QS::BehaviorSet* behaviorSetCreator(
  //   const std::string &theBehaviorSetName,
  //   const QS::PluginEntity::Properties &theProperties,
  //   const std::string &theTag)
  // {
  //   // Only one behavior set in this plugin, so just ignore the name.
  //   return new QS::BasicWalk(theProperties, theTag);
  // }

  // void behaviorSetDestructor(QS::BehaviorSet *theBehaviorSet)
  // {
  //   delete theBehaviorSet;
  // }

  // QS::Behavior* behaviorCreator(
  //   const std::string &theBehaviorName,
  //   const QS::PluginEntity::Properties &theProperties,
  //   const std::string &theTag)
  // {
  //   // Only one Behavior in this plugin, so ignore the name.
  //   return new QS::Walk(theProperties, theTag);
  // }

  // void behaviorDestructor(QS::Behavior *theBehavior)
  // {
  //   delete theBehavior;
  // }

  // QS::Sensor* sensorCreator(
  //   const std::string &theSensorName,
  //   const QS::PluginEntity::Properties &theProperties,
  //   const std::string &theTag)
  // {
  //   // Only one sensor in this plugin, so ignore the name.
  //   return new QS::NullSensor(theProperties, theTag);
  // }

  // void sensorDestructor(QS::Sensor *theSensor)
  // {
  //   delete theSensor;
  // }

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
