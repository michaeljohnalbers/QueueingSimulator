/**
 * @file CollisionAvoidance.cpp
 * @brief Definition of CollisionAvoidance
 *
 * @author Michael Albers (however, most of the algorithms are taken or adapted
 * from Craig Reynolds OpenSteer implementation)
 */

// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------

#include "Actor.h"
#include "CollisionAvoidance.h"
#include "NearestN.h"

QS::CollisionAvoidance::CollisionAvoidance(const Properties &theProperties,
                                           const std::string &theTag) :
  Behavior(theProperties, theTag)
{
}

Eigen::Vector2f QS::CollisionAvoidance::evaluate(const Actor *theActor)
{
  const QS::NearestN *nearestN =
    dynamic_cast<const QS::NearestN*>(getDependencies()[0].myEntity);

  const std::vector<const Actor*> &nearestActors = nearestN->getActors();

  const Actor *actorToAvoid = nullptr;

  // Only care about collisions in the near future.
  float timeUntilApproach = 2.0;

  Eigen::Vector2f otherPositionAtNearestApproach;

  float distanceThreshold = theActor->getRadius() * 2;

  for (auto actor : nearestActors)
  {
    float nearestApproachTime = predictNearestApproachTime(theActor, actor);
    if (nearestApproachTime >= 0 && nearestApproachTime < timeUntilApproach)
    {
      Eigen::Vector2f otherPositionAtNearestApproach2;

      float distance = computeNearestApproachPositions(
        theActor, actor, nearestApproachTime, otherPositionAtNearestApproach2);

      if (distance < distanceThreshold)
      {
        timeUntilApproach = nearestApproachTime;
        actorToAvoid = actor;
        otherPositionAtNearestApproach = otherPositionAtNearestApproach2;
      }
    }
  }

  Eigen::Vector2f steeringForce(0.0, 0.0);
  if (actorToAvoid)
  {
    steeringForce = computeAvoidSteeringForce(theActor, actorToAvoid,
                                              otherPositionAtNearestApproach);
    steeringForce *= theActor->getMass();
  }

  return steeringForce;
}

Eigen::Vector2f QS::CollisionAvoidance::computeAvoidSteeringForce(
  const Actor *theActor,
  const Actor *theOther,
  const Eigen::Vector2f &theOtherPositionAtNearestApproach) const
{
  Eigen::Vector2f steeringForce(0.0, 0.0);

  Eigen::Vector2f velocity = theActor->getVelocity();
  Eigen::Vector2f otherVelocity = theOther->getVelocity();

  // parallel: +1, perpendicular: 0, anti-parallel: -1
  float parallelness = velocity.dot(otherVelocity);
  float angle = 0.707f;

  Eigen::Vector2f side(velocity.y(), -velocity.x());

  float steer = 0.0;
  if (parallelness < -angle)
  {
    // anti-parallel "head on" paths:
    // steer away from future threat position
    Eigen::Vector2f offset =
      theOtherPositionAtNearestApproach - theActor->getPosition();
    float sideDot = offset.dot(side);
    steer = (sideDot > 0) ? -1.0f : 1.0f;
  }
  else
  {
    if (parallelness > angle)
    {
      // parallel paths: steer away from threat
      Eigen::Vector2f offset =
        theOther->getPosition() - theActor->getPosition();
      float sideDot = offset.dot(side);
      steer = (sideDot > 0) ? -1.0f : 1.0f;
    }
    else
    {
      // perpendicular paths: steer behind threat
      // (only the slower of the two does this)
      if (otherVelocity.norm() <= velocity.norm())
      {
        float sideDot = side.dot(otherVelocity);
        steer = (sideDot > 0) ? -1.0f : 1.0f;
      }
    }
  }

  steeringForce = side * steer;

  return steeringForce;
}

float QS::CollisionAvoidance::computeNearestApproachPositions(
  const Actor *theActor,
  const Actor *theOther,
  float theTime,
  Eigen::Vector2f &theOtherPositionAtNearestApproach) const
{
  const Eigen::Vector2f travel = theActor->getVelocity() * theTime;
  const Eigen::Vector2f otherTravel = theOther->getVelocity() * theTime;

  const Eigen::Vector2f final = theActor->getPosition() + travel;
  const Eigen::Vector2f otherFinal = theOther->getPosition() + otherTravel;

  theOtherPositionAtNearestApproach = otherFinal;

  float distance = (final - otherFinal).norm();

  return distance;
}

float QS::CollisionAvoidance::predictNearestApproachTime(const Actor *theActor,
                                                         const Actor *theOther)
  const
{
  // imagine we are at the origin with no velocity,
  // compute the relative velocity of the other vehicle
  const Eigen::Vector2f velocity = theActor->getVelocity();
  const Eigen::Vector2f otherVelocity = theOther->getVelocity();
  const Eigen::Vector2f relVelocity = otherVelocity - velocity;
  const float relSpeed = relVelocity.norm();

  // for parallel paths, the vehicles will always be at the same distance,
  // so return 0 (aka "now") since "there is no time like the present"
  if (0.0 == relSpeed)
  {
    return 0.0;
  }

  // Now consider the path of the other vehicle in this relative
  // space, a line defined by the relative position and velocity.
  // The distance from the origin (our vehicle) to that line is
  // the nearest approach.

  // Take the unit tangent along the other vehicle's path
  const Eigen::Vector2f relTangent = relVelocity / relSpeed;

  // find distance from its path to origin (compute offset from
  // other to us, find length of projection onto path)
  const Eigen::Vector2f relPosition =
    theActor->getPosition() - theOther->getPosition();
  const float projection = relTangent.dot(relPosition);

  return projection / relSpeed;
 
}
