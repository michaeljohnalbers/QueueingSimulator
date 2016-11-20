/**
 * @file GreedyOrdering.cpp
 * @brief Definition of GreedyOrdering
 *
 * @author Michael Albers
 */

#include "CollisionAvoidance.h"
#include "ExitSeek.h"
#include "GreedyOrdering.h"
#include "NearExitArrival.h"
#include "OrderedActor.h"
#include "OrderedExit.h"
#include "Sensable.h"

#include <iostream>
#include <typeinfo>

QS::GreedyOrdering::GreedyOrdering(const Properties &theProperties,
                                   const std::string &theTag) :
  BehaviorSet(theProperties, theTag)
{
}

Eigen::Vector2f QS::GreedyOrdering::evaluate(const Actor *theActor,
                                             const Sensable &theSensable)
{
  populateSensors(theSensable);

  if (! myCollisionAvoidance)
  {
    auto dependencies = getDependencies();
    myCollisionAvoidance = dynamic_cast<QS::CollisionAvoidance*>(
      dependencies[0].myEntity);
    myExitSeek = dynamic_cast<QS::ExitSeek*>(dependencies[1].myEntity);
    myNearExitArrival = dynamic_cast<QS::NearExitArrival*>(
      dependencies[2].myEntity);
  }

  const OrderedActor *orderedActor = dynamic_cast<const OrderedActor*>(
    theActor);
  const OrderedExit *orderedExit = dynamic_cast<const OrderedExit*>(
    theSensable.getExits()[0]);

  static const Eigen::Vector2f zeroVector{0.0, 0.0};

  Eigen::Vector2f steeringForce(0.0, 0.0);

  if (orderedActor->getRank() == orderedExit->getRank())
  {
    steeringForce = myExitSeek->evaluate(theActor);
  }
  else
  {
    Eigen::Vector2f collisionSteering =
      myCollisionAvoidance->evaluate(theActor);
    Eigen::Vector2f nearExitSteering = myNearExitArrival->evaluate(theActor);
    if (false == myNearExitArrival->getAtPosition())
    {
      if (zeroVector == collisionSteering)
      {
        steeringForce = nearExitSteering;
      }
      else
      {
        steeringForce = collisionSteering;
      }
    }
    else
    {
      // Occasionally there is a bit of a residual velocity on an Actor when it
      // has come near the exit. This fixes that.
      steeringForce = -theActor->getVelocity();
      steeringForce *= theActor->getMass();
    }
  }

  return steeringForce;
}
