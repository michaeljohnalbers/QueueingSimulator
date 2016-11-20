/**
 * @file ExitFlee.cpp
 * @brief Definition of ExitFlee
 *
 * @author Michael Albers
 */

#include "Actor.h"
#include "BasicBehaviors.h"
#include "Exit.h"
#include "ExitFlee.h"
#include "FindExitSensor.h"

QS::ExitFlee::ExitFlee(const Properties &theProperties,
                       const std::string &theTag) :
  Behavior(theProperties, theTag)
{
}

Eigen::Vector2f QS::ExitFlee::evaluate(const Actor *theActor)
{
  const FindExitSensor *exitSensor =
    dynamic_cast<const QS::FindExitSensor*>(getDependencies()[0].myEntity);

  const Exit *exit = exitSensor->getExit();

  Eigen::Vector2f steeringForce = BasicBehaviors::flee(
    theActor, exit->getPosition(), theActor->getMaximumSpeed());

  return steeringForce;
}
