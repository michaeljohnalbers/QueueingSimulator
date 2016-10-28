/**
 * @file Walk.cpp
 * @brief Definition of Walk
 *
 * @author Michael Albers
 */

#include "Walk.h"

QS::Walk::Walk(const Properties &theProperties,
               const std::string &theTag) :
  Behavior(theProperties, theTag),
  mySpeed_ms(1.0)
{
  auto speedIter = theProperties.find("speed");
  if (speedIter != theProperties.end())
  {
    mySpeed_ms = std::stof(speedIter->second);
  }
}

Eigen::Vector2f QS::Walk::evaluate(
  const Actor *theActor,
  float theIntervalInSeconds)
{
  return {mySpeed_ms * theIntervalInSeconds, 0};
}
