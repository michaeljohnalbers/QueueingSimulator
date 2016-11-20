/**
 * @file BasicBehaviors.cpp
 * @brief Definition of BasicBehaviors
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include <string>
#include "Actor.h"
#include "BasicBehaviors.h"

Eigen::Vector2f QS::BasicBehaviors::arrival(const Actor *theActor,
                                            const Eigen::Vector2f &thePosition,
                                            float theSlowingRadius)
{
  if (theSlowingRadius < 0.0)
  {
    std::string error{"Invalid slowing radius, "};
    error += std::to_string(theSlowingRadius) + ", it cannot be negative.";
    throw std::invalid_argument(error);
  }

  Eigen::Vector2f desiredVelocity =
     thePosition - theActor->getPosition();

  float distance = desiredVelocity.norm();
  Eigen::Vector2f steeringForce(0.0, 0.0);
  if (0.0 != distance)
  {
    desiredVelocity.normalize();
    desiredVelocity *= theActor->getMaximumSpeed();
    if (distance < theSlowingRadius)
    {
      desiredVelocity *= (distance / theSlowingRadius);
    }
    steeringForce = desiredVelocity - theActor->getVelocity();
  }

  steeringForce *= theActor->getMass();
  return steeringForce;
}

Eigen::Vector2f QS::BasicBehaviors::evade(const Actor *theActor,
                                          const Actor *theActorToEvade)
  noexcept
{
  return evadePursuitHelper(theActor, theActorToEvade, true);
}

Eigen::Vector2f QS::BasicBehaviors::evadePursuitHelper(
  const Actor *theActor,
  const Actor *theOtherActor,
  bool theEvade) noexcept
{
  Eigen::Vector2f distance =
    theOtherActor->getPosition() - theActor->getPosition();
  float updatesAhead = distance.norm() / theOtherActor->getMaximumSpeed();
  Eigen::Vector2f futurePosition = theOtherActor->getPosition() +
    (theOtherActor->getVelocity() * updatesAhead);
  Eigen::Vector2f steeringForce;
  if (theEvade)
  {
    steeringForce = flee(theActor, futurePosition, theActor->getMaximumSpeed());
  }
  else
  {
    steeringForce = seek(theActor, futurePosition, theActor->getMaximumSpeed());
  }
  return steeringForce;
}

Eigen::Vector2f QS::BasicBehaviors::flee(const Actor *theActor,
                                         const Eigen::Vector2f &thePosition,
                                         float theDesiredSpeed) noexcept
{
  Eigen::Vector2f desiredVelocity =
     theActor->getPosition() - thePosition;

  return seekFleeHelper(theActor, desiredVelocity, theDesiredSpeed);
}

Eigen::Vector2f QS::BasicBehaviors::pursuit(const Actor *theActor,
                                            const Actor *theActorToEvade)
  noexcept
{
  return evadePursuitHelper(theActor, theActorToEvade, false);
}

Eigen::Vector2f QS::BasicBehaviors::seek(const Actor *theActor,
                                         const Eigen::Vector2f &thePosition,
                                         float theDesiredSpeed) noexcept
{
  Eigen::Vector2f desiredVelocity =
     thePosition - theActor->getPosition();

  return seekFleeHelper(theActor, desiredVelocity, theDesiredSpeed);
}

Eigen::Vector2f QS::BasicBehaviors::seekFleeHelper(
  const Actor *theActor,
  Eigen::Vector2f theDesiredVelocity,
  float theDesiredSpeed) noexcept
{
  float length = theDesiredVelocity.norm();

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
    theDesiredVelocity /= length;
  }

  theDesiredVelocity *= theDesiredSpeed;

  Eigen::Vector2f steeringForce = theDesiredVelocity - theActor->getVelocity();

  steeringForce *= theActor->getMass();

  return steeringForce;
}

Eigen::Vector2f QS::BasicBehaviors::separation(
  const Actor *theActor,
  const std::vector<const Actor*> &theActors,
  float theSeparationDistance)
{
  if (theSeparationDistance < 0.0)
  {
    std::string error{"Invalid separation distance, "};
    error += std::to_string(theSeparationDistance) + ", it cannot be negative.";
    throw std::invalid_argument(error);
  }

  Eigen::Vector2f steeringForce(0.0, 0.0);

  float actorCount = 0;
  for (auto actor : theActors)
  {
    if (actor != theActor)
    {
      Eigen::Vector2f offset = actor->getPosition() - theActor->getPosition();
      float distanceSquared = offset.dot(offset);
      steeringForce += (offset / -distanceSquared);
      ++actorCount;
    }
  }

  // Check here in case of empty vector, or vector with only theActor in it.
  if (0 == actorCount)
  {
    return steeringForce;
  }

  steeringForce /= actorCount;

  float length = steeringForce.norm();
  if (length > 0.0)
  {
    steeringForce /= length;
  }

  steeringForce *= theSeparationDistance;
  steeringForce *= theActor->getMass();

  return steeringForce;
}

