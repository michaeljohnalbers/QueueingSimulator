/**
 * @file World.cpp
 * @brief Definition of World
 *
 * @author Michael Albers
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Eigen/Core"
#include "Actor.h"
#include "ActorMetrics.h"
#include "ActorUpdateCallback.h"
#include "EigenHelper.h"
#include "Exit.h"
#include "Metrics.h"
#include "Sensable.h"
#include "World.h"

QS::World::World(Metrics &theMetrics) :
  myMetrics(theMetrics)
{
}

void QS::World::addActor(Actor *theActor)
{
  ++myNumberAttemptedActorAdds;
  checkInitialPlacement(theActor);
  myActors.push_back(theActor);
  myActorsInWorld.push_back(theActor);
}

void QS::World::addExit(Exit *theExit)
{
  if (! isInWorld(*theExit))
  {
    std::ostringstream error;
    error << "Exit at position "
          << theExit->getPosition().format(EigenHelper::prettyPrint)
          << " is not fully within the world bounds given its radius of "
          << std::setprecision(5) << theExit->getRadius() << ".";
    throw std::logic_error(error.str());
  }

  // Not parallelizing this loop as there are likely to be only a few exits in
  // the world and adding parallelization would probably slow this down or at
  // best have no effect.
  for (auto exit : myExits)
  {
    if (exit == theExit)
    {
      std::ostringstream error;
      error << "Attempting to add the Exit at position "
            << theExit->getPosition().format(EigenHelper::prettyPrint)
            << " to the world more than once.";
      throw std::logic_error(error.str());
    }
  }

  myExits.push_back(theExit);
  myExitsForSensable.push_back(theExit);
}

void QS::World::checkInitialPlacement(const Actor *theActor) const
{
  Eigen::Vector2f actorPosition = theActor->getPosition();
  float actorRadius = theActor->getRadius();

  // First check the world boundaries.
  if (! isInWorld(*theActor))
  {
    std::ostringstream error;
    error << "Actor at position "
          << actorPosition.format(EigenHelper::prettyPrint)
          << " is not fully within the world bounds given its radius of "
          << std::setprecision(5) << actorRadius << ".";
    throw std::logic_error(error.str());
  }

  int duplicateIndex = -1;
  std::vector<std::decltype(myActors)::size_type> overlappedActorIndexes;
  auto numActors = myActors.size();

#pragma omp parallel for shared(duplicateIndex, overlappedActorIndexes)
  for (auto actorIndex = 0u; actorIndex < numActors; ++actorIndex)
  {
    Actor *currentActor = myActors[actorIndex];

    // Check if actor is already in the world
    if (theActor == currentActor)
    {
      duplicateIndex = actorIndex;
    }

    // Check if actor overlaps another actor
    bool overlap = checkOverlap(
      currentActor->getPosition(), currentActor->getRadius(),
      actorPosition, actorRadius);
    if (overlap)
    {
#pragma omp critical
      {
        overlappedActorIndexes.push_back(actorIndex);
      }
    }
  }

  if (duplicateIndex >= 0)
  {
    std::ostringstream error;
    error << "On attempted Actor add number " << myNumberAttemptedActorAdds
          << ", the Actor being added (at position "
          << actorPosition.format(EigenHelper::prettyPrint)
          << ") has already been added.";
    throw std::logic_error(error.str());
  }

  if (! overlappedActorIndexes.empty())
  {
    std::ostringstream error;
    error << "On attempted Actor add number " << myNumberAttemptedActorAdds
          << ", the Actor being added at position "
          << actorPosition.format(EigenHelper::prettyPrint)
          << " overlaps with " << overlappedActorIndexes.size()
          << " other Actor(s) at the following position(s):";
    for (auto index : overlappedActorIndexes)
    {
      error << " " << myActors[index]->getPosition().format(
        EigenHelper::prettyPrint);
    }
    error << ".";
    throw std::logic_error(error.str());
  }
}

bool QS::World::checkOverlap(Eigen::Vector2f thePosition1, float theRadius1,
                             Eigen::Vector2f thePosition2, float theRadius2)
  const noexcept
{
  bool overlap = false;

  Eigen::Vector2f distanceVector = thePosition1 - thePosition2;
  float distance = distanceVector.norm();
  if (distance <= theRadius1 + theRadius2)
  {
    overlap = true;
  }
  return overlap;
}

Eigen::Vector2f QS::World::collisionDetection(
  Actor *theActor,
  Eigen::Vector2f theMotionVector) const
{
  float actorRadius = theActor->getRadius();
  Eigen::Vector2f actorPosition = theActor->getPosition();

  // Yes, subtracting from 0.0 is overly verbose. However, as I write this it
  // helps to show that the coordinate is being moved back in from the lower
  // world boundaries.
  if (actorPosition.x() + actorRadius + theMotionVector.x() > myWidth_m)
  {
    theMotionVector.x() = myWidth_m - actorPosition.x() - actorRadius;
  }
  else if ((actorPosition.x() - actorRadius) + theMotionVector.x() < 0.0)
  {
    theMotionVector.x() = 0.0 - (actorPosition.x() - actorRadius);
  }

  if (actorPosition.y() + actorRadius + theMotionVector.y() > myLength_m)
  {
    theMotionVector.y() = myLength_m - actorPosition.y() - actorRadius;
  }
  else if((actorPosition.y() - actorRadius) + theMotionVector.y() < 0.0)
  {
    theMotionVector.y() = 0.0 - (actorPosition.y() - actorRadius);
  }

  // Check for collisions with other Actors.
  for (const Actor *collidedActor : myActorsInWorld)
  {
    // Don't check theActor against itself.
    if (theActor == collidedActor)
    {
      continue;
    }

    if (theMotionVector.x() < EigenHelper::FLOAT_TOLERANCE &&
        theMotionVector.x() > -EigenHelper::FLOAT_TOLERANCE &&
        theMotionVector.y() < EigenHelper::FLOAT_TOLERANCE &&
        theMotionVector.y() > -EigenHelper::FLOAT_TOLERANCE)
    {
      // At this point the Actor is essentially not moving, so just skip the
      // rest of the collision detection and return a motionless motion vector.
      theMotionVector << 0.0, 0.0;
      return theMotionVector;
    }

    // Collision detection/resolution taken from:
    // http://www.gamasutra.com/view/feature/131424/pool_hall_lessons_fast_accurate_.php?page=2

    // Have to recalculate each time as it could have been clipped in the
    // previous iteration.
    float motionVectorLength = theMotionVector.norm();
    Eigen::Vector2f vectorBetweenActors =
      collidedActor->getPosition() - actorPosition;

    // First check if the length of the motion vector is enough to cause an
    // collision.
    float distanceBetweenActors = vectorBetweenActors.norm();
    float sumRadii = collidedActor->getRadius() + actorRadius;
    if (motionVectorLength < distanceBetweenActors - sumRadii)
    {
      continue;
    }

    Eigen::Vector2f N = theMotionVector;
    N.normalize();
    float D = N.dot(vectorBetweenActors);

    // Check if theActor is moving towards collidedActor. If D <= 0, it isn't.
    if (D <= 0.0)
    {
      continue;
    }

    // Check if the closest theActor will get to collidedActor is enough to
    // cause a collision.
    float F = (distanceBetweenActors * distanceBetweenActors) -
      (D * D);
    float sumRadiiSquared = sumRadii * sumRadii;
    if (F >= sumRadiiSquared)
    {
      continue;
    }

    // No such right triange with sides of length sumRadiiSquared and F. Avoids
    // sqrt of a negative number.
    float T = sumRadiiSquared - F;
    if (T < 0)
    {
      continue;
    }

    float distance = D - std::sqrt(T);

    // If the motion vector is less than the clipping distance, then there is
    // no collision.
    if (motionVectorLength < distance)
    {
      continue;
    }

    theMotionVector.normalize();
    theMotionVector *= distance;
  }

  return theMotionVector;
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

void QS::World::finalizeMetrics() const noexcept
{
  myMetrics.finalizeActorMetrics(myActors);
  myMetrics.finalizeSimulationMetrics();
}

const std::vector<QS::Actor*>& QS::World::getActors() const noexcept
{
  return myActors;
}

const std::vector<const QS::Actor*>& QS::World::getActorsInWorld()
  const noexcept
{
  return myActorsInWorld;
}

std::tuple<float, float> QS::World::getDimensions() const noexcept
{
  return std::make_tuple(myWidth_m, myLength_m);
}

const std::vector<QS::Exit*>& QS::World::getExits() const noexcept
{
  return myExits;
}

Eigen::Vector2f QS::World::getRandomActorPosition(float theRadius,
                                                  uint32_t theMaxAttempts)
{
  if (theRadius > myWidth_m || theRadius > myLength_m)
  {
    std::ostringstream error;
    error << "Cannot generate random Actor position, the provided radius, "
          << std::fixed << theRadius
          << ", is larger than the world's width or length.";
    throw std::invalid_argument(error.str());
  }

  std::uniform_real_distribution<float> xDistribution(theRadius,
                                                      myWidth_m - theRadius);
  std::uniform_real_distribution<float> yDistribution(theRadius,
                                                      myLength_m - theRadius);

  // Distributions are guaranteed to fit within the world boundaries, so we
  // just need to check against the other actors.
  bool foundPosition = false;
  Eigen::Vector2f position;
  auto numActors = myActors.size();
  uint32_t attempt = 0;
  while (! foundPosition && attempt < theMaxAttempts)
  {
    float x = getRandomNumber(xDistribution);
    float y = getRandomNumber(yDistribution);
    position << x, y;

    bool foundOverlap = false;
#pragma omp parallel for shared(foundOverlap)
    for (auto index = 0u; index < numActors; ++index)
    {
      Actor *actor = myActors[index];
      bool overlap = checkOverlap(position, theRadius,
                                  actor->getPosition(), actor->getRadius());
      if (overlap)
      {
        foundOverlap = true;
      }
    }

    if (! foundOverlap)
    {
      foundPosition = true;
    }
    else
    {
      ++attempt;
    }
  }

  if (attempt >= theMaxAttempts)
  {
    std::ostringstream error;
    error << "Could not generate random Actor position with radius "
          << std::fixed << theRadius << " in the maximum number of "
          << theMaxAttempts << " attempt(s).";
    throw std::logic_error(error.str());
  }

  return position;
}

float QS::World::getRandomNumber(
  std::uniform_real_distribution<float> &theDistribution) noexcept
{
  return theDistribution(myRNGEngine);
}

void QS::World::initializeActorMetrics() noexcept
{
  myMetrics.initializeActorMetrics(myActors);
}

template<class T>
bool QS::World::isInWorld(const T &theEntity) const noexcept
{
  Eigen::Vector2f position = theEntity.getPosition();
  float radius = theEntity.getRadius();

  bool isInWorld = true;
  if (position.x() - radius < 0.0 ||
      position.x() + radius > myWidth_m ||
      position.y() - radius < 0.0 ||
      position.y() + radius > myLength_m)
  {
    isInWorld = false;
  }
  return isInWorld;
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

bool QS::World::update(float theIntervalInSeconds,
                       ActorUpdateCallback &theActorUpdateCallback)
{
  auto actorIter = myActorsInWorld.begin();

  while (actorIter != myActorsInWorld.end())
  {
    Sensable sensable(*actorIter, myActorsInWorld, myExitsForSensable,
                      theIntervalInSeconds);

    // I don't really like the const cast, but I like the "const Actor*"
    // template type of myActorsInWorld better (to make sure the Sensable and
    // users of the Sensable don't mess with the Actor).
    Actor *actor = const_cast<Actor*>(*actorIter);

    Eigen::Vector2f steeringForce = actor->evaluate(sensable);

    float maxForce = actor->getMaximumForce();
    steeringForce = EigenHelper::truncate(steeringForce, maxForce);

    float mass = actor->getMass();
    // Pretend the steering force is in (g*m)/(s*s) (almost newtons, just
    // not kilograms). Dividing force by mass gives acceleration in m/(s*s).
    Eigen::Vector2f acceleration = steeringForce / mass;

    Eigen::Vector2f currentVelocity = actor->getVelocity();
    float maxSpeed = actor->getMaximumSpeed();
    // Multiplying acceleration by time gives m/s.
    Eigen::Vector2f newVelocity = currentVelocity +
      (acceleration * theIntervalInSeconds);
    newVelocity = EigenHelper::truncate(newVelocity, maxSpeed);

    float newOrientation = actor->getOrientation();
    // If the Actor is staying stationary, the orientation goes to -nan, and
    // this causes the Actor to not be drawn.
    if (newVelocity.norm() != 0.0)
    {
      Eigen::Vector2f base{1.0, 0.0};
      Eigen::Vector2f normalizedVelocity = newVelocity;
      normalizedVelocity.normalize();
      newOrientation = std::acos(normalizedVelocity.dot(base));
    }

    // acos(Dot product) gives a value between 0 & PI. So if the Actor is
    // oriented at, say, 270 degrees (3 * PI / 2 radians) the value is PI/2.
    // This corrects for "downward" pointing Actors. It is a simplification of
    // the code in the question at:
    // http://gamedev.stackexchange.com/questions/45412/understanding-math-used-to-determine-if-vector-is-clockwise-counterclockwise-f
    // The extra math can be removed since the 'base' value above is {1,0}.
    if (newVelocity.y() < 0)
    {
      newOrientation = 2 * M_PI - newOrientation;
    }

    Eigen::Vector2f currentPosition = actor->getPosition();

    // Multiplying velocity by time gives a vector, in meters, to where the
    // Actor, ideally, will move.
    Eigen::Vector2f motionVector = newVelocity * theIntervalInSeconds;

    // Make sure this motion vector doesn't cause any collisions.
    motionVector = collisionDetection(actor, motionVector);
    Eigen::Vector2f newPosition = currentPosition + motionVector;

    float grossDistance = (currentPosition - newPosition).norm();
    myMetrics.getActorMetrics(actor).addGrossDistance(grossDistance);

    actor->setVelocity(newVelocity);
    actor->setPosition(newPosition);
    actor->setOrientation(newOrientation);

    // Check if the Actor has exited.
    bool actorExited = false;
    for (auto exit : myExits)
    {
      exit->update(theIntervalInSeconds);
      if (exit->canActorExit(actor))
      {
        actorExited = true;
        break;
      }
    }

    if (actorExited)
    {
      actorIter = myActorsInWorld.erase(actorIter);
    }
    else
    {
      theActorUpdateCallback.actorUpdate(*actorIter);
      ++actorIter;
    }
  }

  myMetrics.addToElapsedTime(theIntervalInSeconds);

  return myActorsInWorld.empty();
}
