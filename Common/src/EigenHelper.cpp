/**
 * @file EigenHelper.cpp
 * @brief Definition of EigenHelper
 *
 * @author Michael Albers
 */

#include "EigenHelper.h"

const Eigen::IOFormat QS::EigenHelper::prettyPrint(
  8, Eigen::DontAlignCols, "", ",", "", "", "(", ")");

const float QS::EigenHelper::FLOAT_TOLERANCE = 0.0001;

Eigen::Vector2f QS::EigenHelper::truncate(Eigen::Vector2f theVector,
                                          float theMaximumLength) noexcept
{
  if (theMaximumLength >= 0)
  {
    float vectorLength = theVector.norm();
    if (vectorLength > theMaximumLength)
    {
      theVector = theVector * (theMaximumLength / vectorLength);
    }
  }
  return theVector;
}
