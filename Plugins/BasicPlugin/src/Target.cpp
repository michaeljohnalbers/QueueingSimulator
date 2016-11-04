/**
 * @file Target.cpp
 * @brief Definition of Target
 *
 * @author Michael Albers
 */

#include <iomanip>
#include <sstream>
#include "PluginHelper.h"
#include "Target.h"

QS::Target::Target(const Properties &theProperties, const std::string &theTag) :
  PluginEntity(theProperties, theTag)
{
  float x = PluginHelper::getProperty(
    theProperties, "x", true, PluginHelper::toFloat);
  if (x < 0.0)
  {
    throw std::invalid_argument(
      "Invalid x coordinate, " + std::to_string(x) +
      ", must be > 0 in Target constructor.");
  }

  float y = PluginHelper::getProperty(
    theProperties, "y", true, PluginHelper::toFloat);
  if (y < 0.0)
  {
    throw std::invalid_argument(
      "Invalid y coordinate, " + std::to_string(y) +
      ", must be > 0 in Target constructor.");
  }

  myPosition << x, y;

  myRadius_m = PluginHelper::getProperty(
    theProperties, "radius", true, PluginHelper::toFloat);
  if (myRadius_m < 0.0)
  {
    throw std::invalid_argument(
      "Invalid radius " + std::to_string(myRadius_m) +
      ", must be >= 0 in Target constructor.");
  }
}

Eigen::Vector2f QS::Target::getPosition() const noexcept
{
  return myPosition;
}

float QS::Target::getRadius() const noexcept
{
  return myRadius_m;
}
