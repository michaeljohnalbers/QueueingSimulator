/**
 * @file Seek.cpp
 * @brief Implementation of the Seek class.
 *
 * @author Michael Albers
 */

#include <Seek.h>

#include <Individual.h>

//***********************
// Seek::
//***********************
Seek::Seek()
{
}

//***********************
// Seek::
//***********************
Seek::~Seek()
{
}

//***********************
// Seek::
//***********************
Eigen::Vector2f Seek::calculateForce(const Individual &theIndividual,
                                     const NearestN *theNeighbors,
                                     const Eigen::Vector2f *theTarget)
{
  Eigen::Vector2f desiredVelocity(*theTarget - theIndividual.getPosition());
  desiredVelocity.normalize();
  desiredVelocity *= theIndividual.getMaximumSpeed();

  Eigen::Vector2f steeringForce(
    desiredVelocity - theIndividual.getCurrentVelocity());

  return steeringForce;
}
