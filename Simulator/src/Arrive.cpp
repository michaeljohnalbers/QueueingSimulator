/**
 * @file Arrive.cpp
 * @brief Implementation of the Arrive class.
 *
 * @author Michael Albers
 */

#include <Individual.h>
#include <Arrive.h>

//***********************
// Arrive::calculateForce
//***********************
Eigen::Vector2f Arrive::calculateForce(const Individual &theIndividual,
                                       const Eigen::Vector2f &theTarget,
                                       float theSlowingRadius)
{
  Eigen::Vector2f desiredForce(theTarget - theIndividual.getPosition());
  auto desiredForceMagnitude = desiredForce.norm();
  desiredForce.normalize();

  if (desiredForceMagnitude <= theSlowingRadius)
  {
    desiredForce *= (theIndividual.getMaximumForce() *
                     desiredForceMagnitude / theSlowingRadius);
  }
  else
  {
    desiredForce *= theIndividual.getMaximumForce();
  }

  Eigen::Vector2f steeringForce(
    desiredForce - theIndividual.getVelocity());

  return steeringForce;
}
