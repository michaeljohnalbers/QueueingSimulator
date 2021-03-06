/**
 * @file Actor.cpp
 * @brief Definition of Actor
 *
 * @author Michael Albers
 */

#define _USE_MATH_DEFINES // For M_PI
#include <cmath>
#include <stdexcept>
#include "Eigen/Geometry"
#include "Actor.h"
#include "BehaviorSet.h"
#include "EigenHelper.h"
#include "PluginHelper.h"

const Eigen::Vector2f QS::Actor::DEFAULT_ORIENTATION(1, 0);

QS::Actor::Actor(const Properties &theProperties, const std::string &theTag) :
  PluginEntity(theProperties, theTag),
  myColor(1.0, 1.0, 1.0), // Use white as OpenGL clear color is black
  myPosition(0.0, 0.0),
  myVelocity_ms(0.0, 0.0)
{
  myMass_grams = PluginHelper::getProperty(
    theProperties, "mass", true, PluginHelper::toFloat);
  if (myMass_grams <= 0.0)
  {
    throw std::invalid_argument(
      "Invalid mass " + std::to_string(myMass_grams) +
      ", must be > 0 in Actor constructor.");
  }

  myRadius_m = PluginHelper::getProperty(
    theProperties,"radius", true, PluginHelper::toFloat);
  if (myRadius_m <= 0.0)
  {
    throw std::invalid_argument(
      "Invalid radius " + std::to_string(myRadius_m) +
      ", must be > 0 in Actor constructor.");
  }

  myMaximumForce = PluginHelper::getProperty(
    theProperties, "max force", true, PluginHelper::toFloat, 1.0f);
  if (myMaximumForce < 0.0)
  {
    throw std::invalid_argument(
      "Invalid maximum force " + std::to_string(myMaximumForce) +
      ", must be >= 0 in Actor constructor.");
  }

  myMaximumSpeed_ms = PluginHelper::getProperty(
    theProperties, "max speed", true, PluginHelper::toFloat, 1.0f);
  if (myMaximumSpeed_ms < 0.0)
  {
    throw std::invalid_argument(
      "Invalid maximum speed " + std::to_string(myMaximumSpeed_ms) +
      ", must be >= 0 in Actor constructor.");
  }

  myOrientation_radians = PluginHelper::getProperty(
    theProperties, "orientation", false, PluginHelper::toFloat);

  setPositionFromProperty();
  setColorFromProperty();
}

Eigen::Vector2f QS::Actor::convertPointToLocal(const Eigen::Vector2f &thePoint)
  const noexcept
{
  Eigen::Vector2f localPoint;

  localPoint.x() =
    (thePoint.x() - myPosition.x()) * std::cos(myOrientation_radians) +
    (thePoint.y() - myPosition.y()) * std::sin(myOrientation_radians);

  localPoint.y() =
    (thePoint.x() - myPosition.x()) * std::sin(myOrientation_radians) * -1.0 +
    (thePoint.y() - myPosition.y()) * std::cos(myOrientation_radians);

  return localPoint;
}

Eigen::Vector2f QS::Actor::evaluate(const Sensable &theSensable)
{
  BehaviorSet *behaviorSet = selectBehaviorSet(theSensable);
  return behaviorSet->evaluate(this, theSensable);
}

Eigen::Vector3f QS::Actor::getColor() const noexcept
{
  return myColor;
}

float QS::Actor::getMass() const noexcept
{
  return myMass_grams;
}

float QS::Actor::getMaximumForce() const noexcept
{
  return myMaximumForce;
}

float QS::Actor::getMaximumSpeed() const noexcept
{
  return myMaximumSpeed_ms;
}

float QS::Actor::getOrientation() const noexcept
{
  return myOrientation_radians;
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

QS::BehaviorSet* QS::Actor::selectBehaviorSet(
  const Sensable &theSensable)
{
  return dynamic_cast<BehaviorSet*>(getDependencies()[0].myEntity);
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

  myColor = PluginHelper::getProperty(
    myProperties, "color", false, toColor, {1,1,1});
}

void QS::Actor::setOrientation(float theOrientationAngle_radians) noexcept
{
  constexpr float MAX_RADIANS = 2 * M_PI;
  theOrientationAngle_radians = std::fmod(theOrientationAngle_radians,
                                          MAX_RADIANS);
  if (theOrientationAngle_radians < 0)
  {
    theOrientationAngle_radians += MAX_RADIANS;
  }
  myOrientation_radians = theOrientationAngle_radians;
}

void QS::Actor::setPosition(const Eigen::Vector2f &thePosition) noexcept
{
  myPosition = thePosition;
}

void QS::Actor::setPositionFromProperty()
{
  myPosition.x() = PluginHelper::getProperty(
    myProperties, "x", true, PluginHelper::toFloat);
  myPosition.y() = PluginHelper::getProperty(
    myProperties, "y", true, PluginHelper::toFloat);
}

void QS::Actor::setVelocity(const Eigen::Vector2f &theVelocity) noexcept
{
  myVelocity_ms = theVelocity;
}
