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
#include "Metrics.h"
#include "Sensable.h"
#include "World.h"

QS::World::World(Metrics &theMetrics) :
  myMetrics(theMetrics)
{
}

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
    error << "Actor at position " << actorPosition.format(QS::prettyPrint)
          << " is not fully within the world bounds given its radius of "
          << std::setprecision(5) << actorRadius << ".";
    throw std::logic_error(error.str());
  }

  for (auto currentActor : myActors)
  {
    // Check if actor is already in the world
    if (theActor == currentActor)
    {
      std::ostringstream error;
      error << "Attempting to add the Actor at position "
            << actorPosition.format(QS::prettyPrint)
            << " to the world more than once. "
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
      error << "Actor at position " << actorPosition.format(QS::prettyPrint)
            << " overlaps with existing Actor at position "
            << currentActorPosition.format(QS::prettyPrint) << ".";
      throw std::logic_error(error.str());
    }
  }
}

Eigen::Vector2f QS::World::collisionDetection(
  Actor *theActor,
  Eigen::Vector2f theNewPosition,
  bool &theCollisionDetected) const
{
  theCollisionDetected = false;

  float actorRadius = theActor->getRadius();

  // Yes, adding 0.0 + actorRadius is meaningless. However, as I write this it
  // helps to show that the coordinate is being moved back in from the lower
  // world boundaries.
  if (theNewPosition.x() + actorRadius > myWidth_m)
  {
    theNewPosition.x() = myWidth_m - actorRadius;
    theCollisionDetected = true;
  }
  else if (theNewPosition.x() - actorRadius < 0.0)
  {
    theNewPosition.x() = 0.0 + actorRadius;
    theCollisionDetected = true;
  }

  if (theNewPosition.y() + actorRadius > myLength_m)
  {
    theNewPosition.y() = myLength_m - actorRadius;
    theCollisionDetected = true;
  }
  else if(theNewPosition.y() - actorRadius < 0.0)
  {
    theNewPosition.y() = 0.0 + actorRadius;
    theCollisionDetected = true;
  }

  for (Actor *collidedActor : myActors)
  {
    // Don't check theActor against itself.
    if (theActor == collidedActor)
    {
      continue;
    }

    Eigen::Vector2f distanceVector =
      collidedActor->getPosition() - theNewPosition;
    float distance = distanceVector.norm();
    float r = collidedActor->getRadius() + actorRadius;
    if (distance <= r)
    {
      theCollisionDetected = true;

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
      if (movement < FLOAT_TOLERANCE)
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
        error << "Unexpected value in collision resolution. discriminant == "
              << std::fixed << discriminant
              << ". Line start: " << lineStart.format(prettyPrint)
              << ". Line end: " << lineEnd.format(prettyPrint)
              << ". Original circle center: "
              << collidedActor->getPosition().format(prettyPrint)
              << ". Radius: " << std::fixed << r;
        // Not all of this information may be displayed in an error dialog so
        // dump it to stderr.
        // TODO: if/when messages are implemented, log this
        std::cerr << error.str() << std::endl;
        throw std::logic_error(error.str());
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

void QS::World::finalizeActorMetrics() const noexcept
{
  myMetrics.finalizeActorMetrics(myActors);
}

const std::vector<QS::Actor*>& QS::World::getActors() const noexcept
{
  return myActors;
}

std::tuple<float, float> QS::World::getDimensions() const noexcept
{
  return std::make_tuple(myWidth_m, myLength_m);
}

void QS::World::initializeActorMetrics() noexcept
{
  myMetrics.initializeActorMetrics(myActors);
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
  Sensable sensable(mySensableActors, theIntervalInSeconds);

  for (Actor *actor : myActors)
  {
    Eigen::Vector2f currentPosition = actor->getPosition();

    Eigen::Vector2f motionVector = actor->calculateMotionVector(sensable);

    Eigen::Vector2f adjustedMotionVector = actor->adjustVectorToMaximums(
      motionVector, theIntervalInSeconds);

    Eigen::Vector2f newPosition = convertPointToWorld(
      actor, adjustedMotionVector);

    bool collision = false;
    newPosition = collisionDetection(actor, newPosition, collision);

    float grossDistance = (currentPosition - newPosition).norm();

    // Check if actor actually moved.
    if (grossDistance > FLOAT_TOLERANCE)
    {
      myMetrics.getActorMetrics(actor).addGrossDistance(grossDistance);

      // This is a bit of a hack. When a collision happens the new position of
      // the actor will be moved, that change can create an artificial rotation
      // and screw up the setting of the orientation. So this fixes the
      // orientation in case of a collision. This may cause problems if an
      // Actor is in the process of curving when a collision happens. I'll
      // cross that bridge when I get there.
      if (! collision)
      {
        auto localPoint = actor->convertPointToLocal(newPosition);
        float theta = std::atan(localPoint.y() / localPoint.x());

        actor->setOrientation(actor->getOrientation() + theta);
      }

      actor->setPosition(newPosition);

      // Have to convert point again since that uses orientation.
      actor->setVelocity(actor->convertPointToLocal(newPosition));
    }
  }
  myMetrics.addToElapsedTime(theIntervalInSeconds);

  return false;
}
