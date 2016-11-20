/**
 * @file Separation.cpp
 * @brief Definition of Separation
 *
 * @author Michael Albers
 */

#include "Actor.h"
#include "BasicBehaviors.h"
#include "NearestN.h"
#include "Separation.h"

QS::Separation::Separation(const Properties &theProperties,
                           const std::string &theTag) :
  Behavior(theProperties, theTag)
{
}

Eigen::Vector2f QS::Separation::evaluate(const Actor *theActor)
{
  const QS::NearestN *nearestN =
    dynamic_cast<const QS::NearestN*>(getDependencies()[0].myEntity);

  Eigen::Vector2f steeringForce = BasicBehaviors::separation(
    theActor, nearestN->getActors(), theActor->getRadius() * 2);
  return steeringForce;
}
