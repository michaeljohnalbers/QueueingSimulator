/**
 * @file Actor.cpp
 * @brief Definition of Actor
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "Actor.h"

QS::Actor::Actor(const Properties &theProperties) :
  PluginEntity(theProperties),
  myPosition(0.0, 0.0),
  myVelocity_ms(0.0, 0.0)
{
  auto massIter = myProperties.find("mass");
  if (myProperties.end() == massIter)
  {
    throw std::invalid_argument(
      "Missing 'mass' property in Actor constructor.");
  }
  myMass_grams = std::stof(massIter->second);
  if (myMass_grams <= 0.0)
  {
    throw std::invalid_argument(
      "Invalid mass " + std::to_string(myMass_grams) +
      ", must be >= 0 in Actor constructor.");
  }

  auto radiusIter = myProperties.find("radius");
  if (myProperties.end() == radiusIter)
  {
    throw std::invalid_argument(
      "Missing 'radius' property in Actor constructor.");
  }
  myRadius_m = std::stof(radiusIter->second);
  if (myRadius_m <= 0.0)
  {
    throw std::invalid_argument(
      "Invalid radius " + std::to_string(myRadius_m) +
      ", must be >= 0 in Actor constructor.");
  }
}

float QS::Actor::getMass() const noexcept
{
  return myMass_grams;
}

Eigen::Vector2f QS::Actor::getPosition() const noexcept
{
  return myPosition;
}

float QS::Actor::getRadius() const noexcept
{
  return myRadius_m;
}

Eigen::Vector2f QS::Actor::getVelocity() const noexcept
{
  return myVelocity_ms;
}

void QS::Actor::setPosition(const Eigen::Vector2f &thePosition) noexcept
{
  myPosition = thePosition;
}

void QS::Actor::setVelocity(const Eigen::Vector2f &theVelocity) noexcept
{
  myVelocity_ms = theVelocity;
}
