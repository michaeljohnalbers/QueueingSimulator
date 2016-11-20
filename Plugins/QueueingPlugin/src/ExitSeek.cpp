/**
 * @file ExitSeek.cpp
 * @brief Definition of ExitSeek
 *
 * @author Michael Albers
 */

#include "Actor.h"
#include "Exit.h"
#include "ExitSeek.h"
#include "FindExitSensor.h"

QS::ExitSeek::ExitSeek(const Properties &theProperties,
                       const std::string &theTag) :
  Behavior(theProperties, theTag)
{
}

Eigen::Vector2f QS::ExitSeek::evaluate(const Actor *theActor)
{
  const FindExitSensor *exitSensor =
    dynamic_cast<const QS::FindExitSensor*>(getDependencies()[0].myEntity);

  const Exit *exit = exitSensor->getExit();

  Eigen::Vector2f desiredVelocity =
     exit->getPosition() - theActor->getPosition();

  float length = desiredVelocity.norm();

  // This avoids some drastic orientation changes when the Actor is sitting at
  // the position.
  if (length <= 0.1)
  {
    return {0.0, 0.0};
  }

  // Eigen's normalize function naturally breaks down when the length is zero.
  // So the by-hand normalize is to save re-doing the norm computation.
  if (length > 0.0)
  {
    desiredVelocity /= length;
  }

  desiredVelocity *= theActor->getMaximumSpeed();

  Eigen::Vector2f steeringForce = desiredVelocity - theActor->getVelocity();

  steeringForce *= theActor->getMass();

  return steeringForce;
}
