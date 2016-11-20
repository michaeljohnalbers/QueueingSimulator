/**
 * @file SemiRationalOrdering.cpp
 * @brief Definition of SemiRationalOrdering
 *
 * @author Michael Albers
 */

#include "CollisionAvoidance.h"
#include "ExitFlee.h"
#include "ExitSeek.h"
#include "OrderedActor.h"
#include "OrderedExit.h"
#include "SemiRationalOrdering.h"
#include "Sensable.h"
#include "Separation.h"

QS::SemiRationalOrdering::SemiRationalOrdering(const Properties &theProperties,
                                               const std::string &theTag) :
  BehaviorSet(theProperties, theTag)
{
}

Eigen::Vector2f QS::SemiRationalOrdering::evaluate(const Actor *theActor,
                                                   const Sensable &theSensable)
{
  populateSensors(theSensable);

  if (! myExitFlee)
  {
    auto dependencies = getDependencies();
    myCollisionAvoidance = dynamic_cast<CollisionAvoidance*>(
      dependencies[0].myEntity);
    myExitFlee = dynamic_cast<ExitFlee*>(dependencies[1].myEntity);
    myExitSeek = dynamic_cast<ExitSeek*>(dependencies[2].myEntity);
    mySeparation = dynamic_cast<Separation*>(dependencies[3].myEntity);
  }

  const OrderedActor *orderedActor = dynamic_cast<const OrderedActor*>(
    theActor);
  const OrderedExit *orderedExit = dynamic_cast<const OrderedExit*>(
    theSensable.getExits()[0]);

  static const Eigen::Vector2f zeroVector{0.0, 0.0};

  Eigen::Vector2f steeringForce(0.0, 0.0);
    Eigen::Vector2f collisionSteering =
      myCollisionAvoidance->evaluate(theActor);
  if (orderedActor->getRank() == orderedExit->getRank())
  {
    if (zeroVector == collisionSteering)
    {
      Eigen::Vector2f exitSeekSteering = myExitSeek->evaluate(theActor);
      steeringForce = exitSeekSteering;
    }
    else
    {
      steeringForce = collisionSteering;
    }
  }
  else
  {
    Eigen::Vector2f separationSteering = mySeparation->evaluate(theActor);
    if (zeroVector == separationSteering)
    {
      steeringForce = theActor->getVelocity() * -1.0 * theActor->getMass();
    }
    else
    {
      Eigen::Vector2f exitFleeSteering = myExitFlee->evaluate(theActor);
      // These multipliers add up to 1.0 and were allocated by mostly
      // guessing that they looked appropriate. Turns out they seem to be.
      steeringForce = (separationSteering * 0.4) +
        (exitFleeSteering * 0.2) +
        (collisionSteering * 0.4);
      steeringForce *= 0.05;
    }
  }

  return steeringForce;
}
