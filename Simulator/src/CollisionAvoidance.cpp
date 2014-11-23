/**
 * @file CollisionAvoidance.cpp
 * @brief Implementation of the CollisionAvoidance class.
 *
 * @author Michael Albers
 */

#include <cfloat>

#include <EigenHelper.h>
#include <Individual.h>
#include <CollisionAvoidance.h>
#include <NearestN.h>

//***********************************
// CollisionAvoidance::calculateForce
//***********************************
Eigen::Vector2f CollisionAvoidance::calculateForce(
  const Individual &theIndividual,
  const NearestN *theNeighbors)
{
  // TODO: Future enhancement: make this random per individual
  // 3 meter look-ahead for determining threats.
  static const float MAX_LOOK_AHEAD = 3.0;

  Eigen::Vector2f velocity(theIndividual.getVelocity());

  float dynamicSpeed = (MAX_LOOK_AHEAD * velocity.norm()) /
    theIndividual.getMaximumSpeed();
  velocity.normalize();
  Eigen::Vector2f lookAhead((theIndividual.getPosition() + velocity) *
                            dynamicSpeed);

  Eigen::Vector2f lookAhead2(lookAhead);
  lookAhead2 *= 0.5;

  auto threat = findImminentThreat(theIndividual,
                                   theNeighbors,
                                   lookAhead,
                                   lookAhead2);

  Eigen::Vector2f steeringForce(0, 0);

  if (nullptr != threat)
  {
    auto threatPosition = threat->getPosition();
    steeringForce[0] = lookAhead[0] - threatPosition[0];
    steeringForce[1] = lookAhead[1] - threatPosition[1];
    steeringForce.normalize();
    steeringForce *= theIndividual.getMaximumForce();
  }

  return steeringForce;
}

//***************************************
// CollisionAvoidance::findImminentThreat
//***************************************
const Individual* CollisionAvoidance::findImminentThreat(
  const Individual &theIndividual,
  const NearestN *theNeighbors,
  const Eigen::Vector2f &theLookAhead,
  const Eigen::Vector2f &theLookAhead2)
{
  const Individual *threat = nullptr;
  float threatDistance = FLT_MAX;

  auto position = theIndividual.getPosition();
  auto neighbors = theNeighbors->getNeighbors();

  for (auto neighbor : neighbors)
  {
    auto neighborPosition = neighbor->getPosition();
    if (EigenHelper::distance(neighborPosition, theLookAhead) <
        neighbor->getBodyRadius() ||
        EigenHelper::distance(neighborPosition, theLookAhead2) <
        neighbor->getBodyRadius())
    {
      float distance = EigenHelper::distance(threat->getPosition(),
                                             position);

      if (nullptr == threat || distance < threatDistance)
      {
        threatDistance = distance;
        threat = neighbor;
      }
    }
  }

  return threat;
}
