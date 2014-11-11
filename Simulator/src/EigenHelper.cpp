/**
 * @file EigenHelper.cpp
 * @brief Implementation of EigenHelper class.
 *
 * @author Michael Albers
 */

#include <cmath>

#include <EigenHelper.h>

//*******************
// EigenHelper::distance
//*******************
float EigenHelper::distance(const Eigen::Vector2f &theFirstPoint,
                            const Eigen::Vector2f &theSecondPoint)
{
  float distance =
    std::sqrt(std::pow(theFirstPoint[0] - theSecondPoint[0], 2) +
              std::pow(theFirstPoint[1] - theSecondPoint[1], 2));
  return distance;
}

//*******************
// EigenHelper::print
//*******************
std::string EigenHelper::print(const Eigen::Vector2f &theVector)
{
  return "(" + std::to_string(theVector[0]) + ", " +
    std::to_string(theVector[1]) + ")";
}

//**********************
// EigenHelper::truncate
//**********************
Eigen::Vector2f EigenHelper::truncate(const Eigen::Vector2f &theVector,
                                      float theMaximumMagnitude)
{
  Eigen::Vector2f truncatedVector(theVector);
  float magnitude = theVector.norm();
  if (magnitude > theMaximumMagnitude)
  {
    truncatedVector *= (theMaximumMagnitude / magnitude);
  }
  return truncatedVector;
}
