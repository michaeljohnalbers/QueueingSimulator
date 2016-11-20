/**
 * @file NearExitArrival.cpp
 * @brief Definition of NearExitArrival
 *
 * @author Michael Albers
 */

#include "Actor.h"
#include "BasicBehaviors.h"
#include "EigenHelper.h"
#include "Exit.h"
#include "FindExitSensor.h"
#include "NearExitArrival.h"
#include "PluginHelper.h"

QS::NearExitArrival::NearExitArrival(const Properties &theProperties,
                                     const std::string &theTag) :
  Behavior(theProperties, theTag)
{
  myDesiredDistanceFromExit = PluginHelper::getProperty(
    theProperties, "distance", false, PluginHelper::toFloat, -1.0f);
}

Eigen::Vector2f QS::NearExitArrival::evaluate(const Actor *theActor)
{
  const FindExitSensor *exitSensor =
    dynamic_cast<const QS::FindExitSensor*>(getDependencies()[0].myEntity);

  const Exit *exit = exitSensor->getExit();

  float desiredDistanceFromExit = myDesiredDistanceFromExit;
  if (myDesiredDistanceFromExit < 0.0)
  {
    // Radius + exit diameter. So exit diameter from exit boundary.
    desiredDistanceFromExit = exit->getRadius() * 3;
  }
  float slowingRadius = desiredDistanceFromExit * theActor->getMaximumSpeed();

  // Find the point to arrive at.
  Eigen::Vector2f toExit = exit->getPosition() - theActor->getPosition();
  float distanceToExit = toExit.norm();

  Eigen::Vector2f desiredPosition = theActor->getPosition();
  if (distanceToExit > desiredDistanceFromExit)
  {
    toExit = EigenHelper::truncate(toExit,
                                   distanceToExit - desiredDistanceFromExit);
    desiredPosition += toExit;
  }
  else
  {
    myAtPosition = true;
  }

  Eigen::Vector2f steeringForce = BasicBehaviors::arrival(
    theActor, desiredPosition, slowingRadius);

  return steeringForce;
}

bool QS::NearExitArrival::getAtPosition() const noexcept
{
  return myAtPosition;
}
