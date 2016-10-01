/**
 * @file Walk.cpp
 * @brief Definition of Walk
 *
 * @author Michael Albers
 */

#include "Walk.h"

QS::Walk::Walk(const Properties &theProperties) :
  Behavior(theProperties)
{
}

Eigen::Vector2f QS::Walk::evaluate(
  const Actor *theActor,
  const std::chrono::milliseconds &theInterval)
{
  float intervalInSeconds = theInterval.count() / 1000.0;
  // 2 MPH converted to meters/second.
  float twoMPHAsMperSec = 0.89408;
  return {twoMPHAsMperSec * intervalInSeconds, 0};
}
