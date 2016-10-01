/**
 * @file World.cpp
 * @brief Definition of World
 *
 * @author Michael Albers
 */

#include <cmath>
#include "Eigen/Core"
#include "Actor.h"
#include "Sensable.h"
#include "World.h"

void QS::World::addActor(Actor *theActor)
{
  // TODO: need to verify this Actor does not overlap with any other Actor, and
  // that it is fully within the world bounds
  myActors.push_back(theActor);
  mySensableActors.push_back(theActor);
}

Eigen::Vector2f QS::World::convertPointToWorld(const Actor *theActor,
                                               const Eigen::Vector2f &thePoint)
  noexcept
{
  Eigen::Vector2f worldPoint;

  Eigen::Vector2f actorPosition = theActor->getPosition();
  auto actorOrientation = theActor->getOrientation();

  worldPoint.x() =
    (thePoint.x() * std::cos(actorOrientation)) -
    (thePoint.y() * std::sin(actorOrientation)) + actorPosition.x();

  worldPoint.y() =
    (thePoint.x() * std::sin(actorOrientation)) -
    (thePoint.y() * std::cos(actorOrientation)) + actorPosition.y();

  return worldPoint;
}

const std::vector<QS::Actor*>& QS::World::getActors() const noexcept
{
  return myActors;
}

std::tuple<float, float> QS::World::getDimensions() const noexcept
{
  return std::make_tuple(myWidth_m, myLength_m);
}

void QS::World::setDimentions(float theWidth_m, float theLength_m)
{
  myWidth_m = theWidth_m;
  myLength_m = theLength_m;
}

void QS::World::setSeed(uint64_t theSeed)
{
  myRNGEngine.seed(theSeed);
}

bool QS::World::update(std::chrono::milliseconds theInterval)
{
  Sensable sensable(mySensableActors, theInterval);

  for (Actor *actor : myActors)
  {
    Eigen::Vector2f motionVector = actor->calculateMotionVector(sensable);

    Eigen::Vector2f adjustedMotionVector = actor->adjustVectorToMaximums(
      motionVector, theInterval);

    Eigen::Vector2f newActorPosition = convertPointToWorld(
      actor, adjustedMotionVector);

    // TODO: Collision detection: world boundaries, other Actors

    actor->setPosition(newActorPosition);
    // TODO: need to update Actor's velocity, orientation
  }

  return false;
}
