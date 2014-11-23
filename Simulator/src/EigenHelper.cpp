/**
 * @file EigenHelper.cpp
 * @brief Implementation of EigenHelper class.
 *
 * @author Michael Albers
 */

#include <iomanip>
#include <cmath>
#include <sstream>

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
  std::ostringstream position;
  position << "(" << std::setprecision(6) << theVector[0] << ", "
           << theVector[1] << ")";
  return position.str();
}

//**********************
// EigenHelper::truncate
//**********************
Eigen::Vector2f EigenHelper::truncate(const Eigen::Vector2f &theVector,
                                      float theMaximumLength)
{
  // Math taken from OpenSteer::Vec3::truncateLength (from OpenSteer package
  // see opensteer.sourceforge.net.
  float maxLengthSquared = theMaximumLength * theMaximumLength;
  float vectorLengthSquared = theVector.dot(theVector);
  Eigen::Vector2f truncatedVector(theVector);
  if (vectorLengthSquared > maxLengthSquared)
  {
    truncatedVector *= (theMaximumLength / std::sqrt(vectorLengthSquared));
  }
  return truncatedVector;
}
