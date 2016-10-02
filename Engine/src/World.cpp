/**
 * @file World.cpp
 * @brief Definition of World
 *
 * @author Michael Albers
 */

#include <cmath>
#include <iomanip>
#include <sstream>
#include "Eigen/Core"
#include "Actor.h"
#include "EigenHelper.h"
#include "Sensable.h"
#include "World.h"

void QS::World::addActor(Actor *theActor)
{
  checkInitialPlacement(theActor);
  myActors.push_back(theActor);
  mySensableActors.push_back(theActor);
}

void QS::World::checkInitialPlacement(const Actor *theActor) const
{
  Eigen::Vector2f actorPosition = theActor->getPosition();
  float actorRadius = theActor->getRadius();

  // First check the world boundaries.
  if (actorPosition.x() - actorRadius < 0.0 ||
      actorPosition.x() + actorRadius > myWidth_m ||
      actorPosition.y() - actorRadius < 0.0 ||
      actorPosition.y() + actorRadius > myLength_m)
  {
    std::ostringstream error;
    error << "Actor at position (" << actorPosition.format(QS::prettyPrint)
          << ") is not fully within the world bounds given its radius of "
          << std::setprecision(5) << actorRadius << ".";
    throw std::logic_error(error.str());
  }

  for (auto currentActor : myActors)
  {
    // Check if actor is already in the world
    if (theActor == currentActor)
    {
      std::ostringstream error;
      error << "Attempting to add the Actor at position ("
            << actorPosition.format(QS::prettyPrint)
            << ") to the world more than once. "
            << "Can only add the Actor once.";
      throw std::logic_error(error.str());
    }

    // Check if actor overlaps another actor
    Eigen::Vector2f currentActorPosition = currentActor->getPosition();
    Eigen::Vector2f distanceVector = currentActorPosition - actorPosition;
    float distance = std::abs(distanceVector.norm());
    if (distance <= actorRadius + currentActor->getRadius())
    {
      std::ostringstream error;
      error << "Actor at position (" << actorPosition.format(QS::prettyPrint)
            << ") overlaps with existing Actor at position ("
            << currentActorPosition.format(QS::prettyPrint) << ").";
      throw std::logic_error(error.str());
    }
  }
}

void QS::World::collisionDetection(Actor *theActor,
                                   const Eigen::Vector2f theNewPosition) const
{
  float actorRadius = theActor->getRadius();

  // TODO: check against world boundary

  std::vector<Actor*> collidedActors;
  for (Actor *actor : myActors)
  {
    // Don't check theActor against itself.
    if (theActor != actor)
    {
      Eigen::Vector2f distanceVector = actor->getPosition() - theNewPosition;
      float distance = std::abs(distanceVector.norm());
      if (distance <= actorRadius + actor->getRadius())
      {
        collidedActors.push_back(actor);
      }
    }
  }

  for (Actor *collidedActor : collidedActors)
  {
  }
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

void QS::World::setDimensions(float theWidth_m, float theLength_m)
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

    collisionDetection(actor, newActorPosition);

    actor->setPosition(newActorPosition);
    // TODO: need to update Actor's velocity, orientation
  }

  return false;
}
