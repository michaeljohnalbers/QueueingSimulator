/**
 * @file Flee.cpp
 * @brief Implementation of the Flee class.
 *
 * @author Michael Albers
 */

#include <Individual.h>
#include <Flee.h>

//*********************
// Flee::calculateForce
//*********************
Eigen::Vector2f Flee::calculateForce(const Individual &theIndividual,
                                     const Eigen::Vector2f &theTarget)
{
  Eigen::Vector2f desiredForce(theIndividual.getPosition() - theTarget);
  desiredForce.normalize();
  // Reynolds' version of seek uses max speed to determine the desired
  // direction/speed vector (which he called desired velocity). I changed this
  // to force to line up with the physics/math of the calculations. Which
  // means, of course, that steering force is actually a force.
  desiredForce *= theIndividual.getMaximumForce();

  // Having said that above, this technically isn't correct. But it works.
  Eigen::Vector2f steeringForce(
    desiredForce - theIndividual.getVelocity());

  return steeringForce;
}
