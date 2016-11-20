/**
 * @file LooseOrdering.cpp
 * @brief Definition of LooseOrdering
 *
 * @author Michael Albers
 */

#include "CollisionAvoidance.h"
#include "ExitSeek.h"
#include "LooseOrdering.h"
#include "OrderedActor.h"
#include "OrderedExit.h"
#include "OrderedLeaderFollow.h"
#include "Sensable.h"

QS::LooseOrdering::LooseOrdering(const Properties &theProperties,
                                 const std::string &theTag) :
  BehaviorSet(theProperties, theTag)
{
}

Eigen::Vector2f QS::LooseOrdering::evaluate(const Actor *theActor,
                                            const Sensable &theSensable)
{
  // TODO: move into Engine?
  populateSensors(theSensable);

  if (! myCollisionAvoidance)
  {
    auto dependencies = getDependencies();
    myCollisionAvoidance = dynamic_cast<QS::CollisionAvoidance*>(
      getDependencies()[0].myEntity);
    myExitSeek = dynamic_cast<QS::ExitSeek*>(
      getDependencies()[1].myEntity);
    myLeaderFollow = dynamic_cast<QS::OrderedLeaderFollow*>(
      getDependencies()[2].myEntity);
  }

  Eigen::Vector2f collisionSteering = myCollisionAvoidance->evaluate(theActor);
  Eigen::Vector2f exitSteering = myExitSeek->evaluate(theActor);
  Eigen::Vector2f leaderFollowSteering = myLeaderFollow->evaluate(theActor);

  const OrderedActor *orderedActor = dynamic_cast<const OrderedActor*>(
    theActor);
  const OrderedExit *orderedExit = dynamic_cast<const OrderedExit*>(
    theSensable.getExits()[0]);

  auto exitRank = orderedExit->getRank();
  auto actorRank = orderedActor->getRank() - exitRank;

  Eigen::Vector2f steeringForce;
  static const Eigen::Vector2f zeroVector{0.0, 0.0};
  if (zeroVector == collisionSteering || 0 == actorRank)
  {
    if (0 == actorRank)
    {
      steeringForce = exitSteering;
    }
    else
    {
      steeringForce = leaderFollowSteering;

      if (zeroVector == steeringForce)
      {
        // No leader, stop.
        // TODO: future work could add some more "intelligence" and direct the
        // Actor to move slowly toward the exit. This is quite difficult to
        // implement correctly.
        steeringForce = -theActor->getVelocity();
        steeringForce *= theActor->getMass();
      }
    }
  }
  else
  {
    steeringForce = collisionSteering;
  }

  return steeringForce;
}
