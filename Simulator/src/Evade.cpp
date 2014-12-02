/**
 * @file Evade.cpp
 * @brief Implementation of the Evade class.
 *
 * @author Michael Albers
 */

#include <EigenHelper.h>
#include <Evade.h>
#include <Flee.h>
#include <Individual.h>

//**********************
// Evade::calculateForce
//**********************
Eigen::Vector2f Evade::calculateForce(const Individual &theIndividual,
                                      const Individual &theIndividualEvade)
{
  Eigen::Vector2f distance(theIndividualEvade.getPosition() -
                           theIndividual.getPosition());
  int32_t updatesAhead = distance.norm() / theIndividual.getMaximumSpeed();
  Eigen::Vector2f futurePosition = theIndividualEvade.getPosition() +
    theIndividualEvade.getVelocity() * updatesAhead;
  return Flee::calculateForce(theIndividualEvade, futurePosition);
}
