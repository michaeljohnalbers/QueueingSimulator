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
  float theIntervalInSeconds)
{
  float speedInMetersPerSecond = 1.0;
  return {speedInMetersPerSecond * theIntervalInSeconds, 0};
}
