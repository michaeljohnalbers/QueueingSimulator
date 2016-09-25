/**
 * @file Actor.cpp
 * @brief Definition of Actor
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "Actor.h"

static std::function<float(const std::string&)> toFloat =
  [](const std::string &value)
{
  return std::stof(value);
};

QS::Actor::Actor(const Properties &theProperties) :
  PluginEntity(theProperties),
  myColor(1.0, 1.0, 1.0), // Use white as OpenGL clear color is black
  myPosition(0.0, 0.0),
  myVelocity_ms(0.0, 0.0)
{
  std::function<float(const std::string&)> toFloat =
    [](const std::string &value)
    {
      return std::stof(value);
    };

  myMass_grams = getProperty("mass", true, toFloat);
  if (myMass_grams <= 0.0)
  {
    throw std::invalid_argument(
      "Invalid mass " + std::to_string(myMass_grams) +
      ", must be > 0 in Actor constructor.");
  }

  myRadius_m = getProperty("radius", true, toFloat);
  if (myRadius_m <= 0.0)
  {
    throw std::invalid_argument(
      "Invalid radius " + std::to_string(myRadius_m) +
      ", must be > 0 in Actor constructor.");
  }

  setPositionFromProperty();
  setColorFromProperty();
}

Eigen::Vector3f QS::Actor::getColor() const noexcept
{
  return myColor;
}

float QS::Actor::getMass() const noexcept
{
  return myMass_grams;
}

Eigen::Vector2f QS::Actor::getPosition() const noexcept
{
  return myPosition;
}

template<class T>
T QS::Actor::getProperty(
  const std::string &thePropertyName, bool theRequired,
  std::function<T(const std::string&)> theConversionFunction) const
{
  T value;
  auto propertyIter = myProperties.find(thePropertyName);
  if (myProperties.end() == propertyIter)
  {
    if (theRequired)
    {
      throw std::invalid_argument(
        "Missing '" + thePropertyName + "' property in Actor constructor.");
    }
  }
  else
  {
    value = theConversionFunction(propertyIter->second);
  }
  return value;
}

float QS::Actor::getRadius() const noexcept
{
  return myRadius_m;
}

Eigen::Vector2f QS::Actor::getVelocity() const noexcept
{
  return myVelocity_ms;
}

void QS::Actor::setColorFromProperty()
{
  std::function<Eigen::Vector3f(const std::string&)> toColor =
    [](const std::string &theValue) -> Eigen::Vector3f
    {
      Eigen::Vector3f color;
      std::stringstream stream(theValue);
      for (auto ii = 0; ii < 3; ++ii)
      {
        float tempColor;
        stream >> tempColor;
        if (tempColor < 0.0 || tempColor > 1.0)
        {
          std::string error{"Invalid '"};
          error += "RGB"[ii] + "' value. Must be 0.0 <= x <= 1.0 in "
            "Actor constructor.";
          throw std::invalid_argument(error);
        }
        color(ii) = tempColor;
      }
      return color;
    };

  myColor = getProperty("color", false, toColor);
}

void QS::Actor::setPosition(const Eigen::Vector2f &thePosition) noexcept
{
  myPosition = thePosition;
}

void QS::Actor::setPositionFromProperty()
{
  float x = getProperty("x", true, toFloat);
  float minX = myRadius_m;
  if (x <= minX)
  {
    throw std::invalid_argument(
      "Invalid X coordinate " + std::to_string(x) +
      ", must be > " + std::to_string(minX) + " in Actor constructor.");
  }
  
  float y = getProperty("y", true, toFloat);
  float minY = myRadius_m;
  if (y <= minY)
  {
    throw std::invalid_argument(
      "Invalid Y coordinate " + std::to_string(y) +
      ", must be > " + std::to_string(minY) + " in Actor constructor.");
  }

  myPosition(0) = x;
  myPosition(1) = y;
}

void QS::Actor::setVelocity(const Eigen::Vector2f &theVelocity) noexcept
{
  myVelocity_ms = theVelocity;
}
