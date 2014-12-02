/**
 * @file RankedLeaderFollow.cpp
 * @brief Implementation of the RankedLeaderFollow class.
 *
 * @author Michael Albers
 */

#include <Arrive.h>
#include <EigenHelper.h>
#include <Evade.h>
#include <Exit.h>
#include <Individual.h>
#include <NearestN.h>
#include <RankedLeaderFollow.h>
#include <Seek.h>

//***********************************
// RankedLeaderFollow::calculateForce
//***********************************
Eigen::Vector2f RankedLeaderFollow::calculateForce(
  const Individual &theIndividual,
  const NearestN &theNeighbors)
{
  // TODO: have this vary by Individual
  static const int32_t LEADER_DISTANCE = 1.5; // Meters

  Eigen::Vector2f steeringForce;

  const Individual *closestRankedNeighbor =
    theNeighbors.getClosestRankedNeighbor();
  if (nullptr != closestRankedNeighbor)
  {
    const auto leaderPosition = closestRankedNeighbor->getPosition();
    Eigen::Vector2f position = closestRankedNeighbor->getVelocity();

    position.normalize();
    position *= LEADER_DISTANCE;

    Eigen::Vector2f aheadPosition(leaderPosition + position);
    Eigen::Vector2f followPosition(position * -1);
    followPosition += leaderPosition;

    if (inWayOfLeader(theIndividual.getPosition(),
                      leaderPosition, aheadPosition))
    {
      steeringForce += Evade::calculateForce(theIndividual,
                                               *closestRankedNeighbor);
    }

    // This is just a semi-thought out number.
    static const float SLOWING_RADIUS = 2;

    steeringForce += Arrive::calculateForce(theIndividual,
                                            followPosition,
                                            SLOWING_RADIUS);
  }
  else
  {
    // TODO: Future enhancement: Make this smarter and have the Individual
    // move towards its approximate place in line.
    steeringForce = Seek::calculateForce(theIndividual, Exit::getPosition());
  }

  return steeringForce;
}

//**********************************
// RankedLeaderFollow::inWayOfLeader
//**********************************
bool RankedLeaderFollow::inWayOfLeader(
  const Eigen::Vector2f &theIndividualPosition,
  const Eigen::Vector2f &theLeaderPosition,
  const Eigen::Vector2f &theLeaderAheadPosition)
{
  static const int32_t LEADER_SIGHT_DISTANCE = 3.0;
  int32_t aheadDistance = EigenHelper::distance(theLeaderAheadPosition,
                                                theIndividualPosition);
  int32_t leaderDistance = EigenHelper::distance(theLeaderPosition,
                                                 theIndividualPosition);
  return (aheadDistance <= LEADER_SIGHT_DISTANCE ||
          leaderDistance <= LEADER_SIGHT_DISTANCE);
}
