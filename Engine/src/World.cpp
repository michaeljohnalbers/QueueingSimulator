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
  Eigen::Vector2f theNewPosition) const
{
  float actorRadius = theActor->getRadius();

  // Yes, adding 0.0 + actorRadius is meaningless. However, as I write this it
  // helps to show that the coordinate is being moved back in from the lower
  // world boundaries.
  if (theNewPosition.x() + actorRadius > myWidth_m)
  {
    theNewPosition.x() = myWidth_m - actorRadius;
  }
  else if (theNewPosition.x() - actorRadius < 0.0)
  {
    theNewPosition.x() = 0.0 + actorRadius;
  }

  if (theNewPosition.y() + actorRadius > myLength_m)
  {
    theNewPosition.y() = myLength_m - actorRadius;
  }
  else if(theNewPosition.y() - actorRadius < 0.0)
  {
    theNewPosition.y() = 0.0 + actorRadius;
  }

  for (const Actor *collidedActor : myActorsInWorld)
  {
    // Don't check theActor against itself.
    if (theActor == collidedActor)
    {
      continue;
    }

    // Not using checkOverlap since the radius sum will be used later on. No
    // sense calculating it twice (or using the extra function overhead either.
    Eigen::Vector2f distanceVector =
      collidedActor->getPosition() - theNewPosition;
    float distance = distanceVector.norm();
    float r = collidedActor->getRadius() + actorRadius;
    if (distance < r)
    {
#if 0
      Eigen::Vector2f motionVector = theNewPosition - theActor->getPosition();
      motionVector.normalize();
      distanceVector.normalize();
      auto cosTheta = motionVector.dot(distanceVector);
      auto theta = std::acos(cosTheta);

      Eigen::Vector2f motionVector2 = theActor->getPosition() - theNewPosition;
      motionVector2.normalize();
      auto cosTheta2 = motionVector2.dot(distanceVector);
      auto theta2 = std::acos(cosTheta2);

      // Only check collisions happening on the "front" of the circle (i.e.,
      // the 180 degrees in the direction of motion)
      std::cout << "Theta: " << std::fixed << theta
                << ", theta2: " << std::fixed << theta2
                << " (" << std::fixed << (M_PI/2) << "," << std::fixed
                << (3*M_PI/2) << ")"
                << ". Actor: " << theActor->getProperties()["color"]
                << ". Collided: "<< collidedActor->getProperties()["color"]
                << std::endl;
      if (theta >= M_PI/2 && theta < 3 * M_PI / 2)
      {
        continue;
      }
#endif

      // The current Actor (A1) has collided with some other Actor (A2).
      //
      // A1 needs to be backed up to the point where it is just touching
      // A2. (That is, the position at which A1 has bumped into A2.) The line
      // from A1's former position to its current (albeit invalid) position is
      // used. This line will intersect with a circle of radius R1 + R2 (A1's
      // radius + A2's radius) centered at A2's center.
      //
      // The circumference of this circle is all points where A1's center can
      // be such that A1 is just touching A2. For the collision resolution the
      // points of intersection with the line are needed.
      //
      // The line will intersect the circle at 1 or 2 places. If just one
      // point, A1 will be moved to this spot. If two points A1 will be moved
      // to the closest point to A1's former position. This moves A1 to the
      // furthest valid position along its desired path of motion.


      // The following is taken from
      // http://mathworld.wolfram.com/Circle-LineIntersection.html
      // This is based off a circle at (0,0), hence the initial vector
      // subtraction and later addition. (I did try it without these
      // corrections thinking that since all points are equally relative to
      // each other regardless of the circle origin the math would work out. It
      // doesn't.)

      // Actor hasn't moved (or essentially hasn't moved) or has been adjusted
      // to the point of not moving (which is why this check is here and not up
      // top). No further need to check for collisions. When there is no change
      // in position, the line/circle intersection code breaks down.
      float movement = (theActor->getPosition() - theNewPosition).norm();
      if (movement < EigenHelper::FLOAT_TOLERANCE)
      {
        break;
      }

      Eigen::Vector2f lineStart =
        theActor->getPosition() - collidedActor->getPosition();
      Eigen::Vector2f lineEnd =
        theNewPosition - collidedActor->getPosition();

      float dx = lineEnd.x() - lineStart.x();
      float dy = lineEnd.y() - lineStart.y();
      float dr = std::sqrt((dx * dx) + (dy * dy));
      float D = (lineStart.x() * lineEnd.y()) - (lineEnd.x() * lineStart.y());
      float drSquared = dr * dr;
      float discriminant = ((r * r) * (drSquared)) - (D * D);

      // This should never happen, but if it does, need to make sure there is
      // enough information for diagnosis.
      if (discriminant < 0.0f)
      {
        std::ostringstream error;
        error << "Unexpected value in collision resolution. " << std::endl
              << "  discriminant == " << std::fixed << discriminant << std::endl
              << "  Line start: " << lineStart.format(EigenHelper::prettyPrint)
              << std::endl
              << "  Line end: " << lineEnd.format(EigenHelper::prettyPrint)
              << std::endl
              << "  Original circle center: "
              << collidedActor->getPosition().format(EigenHelper::prettyPrint)
              << std::endl
              << "  Radius: " << std::fixed << r;
        // Not all of this information may be displayed in an error dialog so
        // dump it to stderr.
        // TODO: if/when messages are implemented, log this
        std::cerr << error.str() << std::endl;

        // Something went wrong with the math, try to keep going by just keeping
        // the Actor in place.
        theNewPosition = theActor->getPosition();
        break;
      }

      auto sgn = [](float val) {return (val < 0.0f ? -1.0f : 1.0);};

      float discriminantSqrt = std::sqrt(discriminant);

      float x1 = ((D * dy) + (sgn(dy) * dx * discriminantSqrt)) /
        drSquared;
      float x2 = ((D * dy) - (sgn(dy) * dx * discriminantSqrt)) /
        drSquared;

      float y1 = ((-D * dx) + (std::abs(dy) * discriminantSqrt)) /
        drSquared;
      float y2 = ((-D * dx) - (std::abs(dy) * discriminantSqrt)) /
        drSquared;

      Eigen::Vector2f intersection1(x1, y1);
      Eigen::Vector2f intersection2(x2, y2);

      intersection1 += collidedActor->getPosition();
      intersection2 += collidedActor->getPosition();

      float d1 = (theActor->getPosition() - intersection1).norm();
      float d2 = (theActor->getPosition() - intersection2).norm();

      if (d1 < d2)
      {
        theNewPosition = intersection1;
      }
      else
      {
        theNewPosition = intersection2;
      }
    }
  }

  return theNewPosition;
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

bool QS::World::update(float theIntervalInSeconds)
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

    Eigen::Vector2f base{1.0, 0.0};
    Eigen::Vector2f normalizedVelocity = newVelocity;
    normalizedVelocity.normalize();
    float newOrientation = std::acos(normalizedVelocity.dot(base));

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
    // Finally, multiplying velocity by time gives meters.
    Eigen::Vector2f newPosition = currentPosition +
      (newVelocity * theIntervalInSeconds);

    newPosition = collisionDetection(actor, newPosition);

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
      ++actorIter;
    }
  }

  myMetrics.addToElapsedTime(theIntervalInSeconds);

  return myActorsInWorld.empty();
}
