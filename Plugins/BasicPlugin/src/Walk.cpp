/**
 * @file Walk.cpp
 * @brief Definition of Walk
 *
 * @author Michael Albers
 */

#include "Actor.h"
#include "Walk.h"

QS::Walk::Walk(const Properties &theProperties,
               const std::string &theTag) :
  Behavior(theProperties, theTag),
  mySpeed_ms(1.0)
{
  auto speedIter = theProperties.find("speed");
  if (speedIter != theProperties.end())
  {
    mySpeed_ms = std::stof(speedIter->second);
  }
}

Eigen::Vector2f QS::Walk::evaluate(const Actor *theActor)
{
  Eigen::Vector2f desiredVelocity(mySpeed_ms, 0);
  Eigen::Vector2f velocity = theActor->getVelocity();
  if (velocity.norm() >= mySpeed_ms)
  {
    desiredVelocity << 0.0, 0.0;
  }
  Eigen::Vector2f steeringForce = desiredVelocity *
    theActor->getMass();
  return steeringForce;
}
