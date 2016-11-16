/**
 * @file OrderedLeaderFollow.cpp
 * @brief Definition of OrderedLeaderFollow
 *
 * Adapted from:
 * https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-leader-following--gamedev-10810
 *
 * @author Michael Albers
 */

#include "Actor.h"
#include "BasicBehaviors.h"
#include "NearestN.h"
#include "OrderedActor.h"
#include "OrderedLeaderFollow.h"

QS::OrderedLeaderFollow::OrderedLeaderFollow(const Properties &theProperties,
                                             const std::string &theTag) :
  Behavior(theProperties, theTag)
{
}

Eigen::Vector2f QS::OrderedLeaderFollow::evaluate(const Actor *theActor)
{
  const QS::NearestN *nearestN =
    dynamic_cast<const QS::NearestN*>(getDependencies()[0].myEntity);

  const OrderedActor *orderedActor = dynamic_cast<const OrderedActor*>(
    theActor);

  const std::vector<const Actor*> &nearestActors = nearestN->getActors();

  // First, find the closest, higher ranked Actor
  uint32_t smallestRankDifference = std::numeric_limits<uint32_t>::max();
  const Actor *nearestRankedActor = nullptr;
  for (auto actor : nearestActors)
  {
    const OrderedActor *orderedOtherActor =
      dynamic_cast<const OrderedActor*>(actor);

    if (orderedOtherActor->getRank() < orderedActor->getRank())
    {
      uint32_t rankDifference =
        orderedActor->getRank() - orderedOtherActor->getRank();
      if (rankDifference < smallestRankDifference)
      {
        smallestRankDifference = rankDifference;
        nearestRankedActor = actor;
      }
    }
  }

  Eigen::Vector2f steeringForce(0.0, 0.0);
  if (nearestRankedActor != nullptr)
  {
    // Find point behind leader to move toward.
    float distanceDiff = nearestRankedActor->getRadius() * 3;

    Eigen::Vector2f vector = nearestRankedActor->getVelocity();
    float length = vector.norm();
    if (length > 0.0)
    {
      vector /= length;
    }
    vector *= distanceDiff;

    Eigen::Vector2f ahead = nearestRankedActor->getPosition() + vector;
    vector *= -1;
    Eigen::Vector2f behind = nearestRankedActor->getPosition() + vector;

    float radius = 2.0;

    auto distance = [](const Eigen::Vector2f &a, const Eigen::Vector2f &b)
    {
      float x = a.x() - b.x();
      float y = a.y() - b.y();
      return std::sqrt(x * x + y * y);
    };

    if (distance(ahead, theActor->getPosition()) <= radius ||
        distance(nearestRankedActor->getPosition(),
                 theActor->getPosition()) <= radius)
    {
      steeringForce += BasicBehaviors::evade(theActor, nearestRankedActor);
    }

    steeringForce += BasicBehaviors::arrival(theActor, behind, 2.0);
    steeringForce += BasicBehaviors::separation(theActor, nearestActors, 0.1);
  }

  //steeringForce *= theActor->getMass();

  return steeringForce;
}
