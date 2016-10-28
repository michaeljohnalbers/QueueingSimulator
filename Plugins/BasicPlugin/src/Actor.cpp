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

const Eigen::Vector2f QS::Actor::DEFAULT_ORIENTATION(1, 0);

static std::function<float(const std::string&)> toFloat =
  [](const std::string &value)
{
  return std::stof(value);
};

QS::Actor::Actor(const Properties &theProperties, const std::string &theTag) :
  PluginEntity(theProperties, theTag),
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

  myOrientation_radians = getProperty("orientation", false, toFloat);
  myMaximumRotationSpeed_rs = getProperty("max rotation", false, toFloat,
                                          -1.0f);
  myMaximumSpeed_ms = getProperty("max speed", false, toFloat, -1.0f);

  setPositionFromProperty();
  setColorFromProperty();
}

Eigen::Vector2f QS::Actor::adjustVectorToMaximums(
  const Eigen::Vector2f &theVector,
  float theIntervalInSeconds) const noexcept
{
  Eigen::Vector2f adjustedVector = theVector;

  // Don't use member in case derived class overrides getMaximumSpeed.
  float maximumSpeed = getMaximumSpeed();

  // Shotcut for fixed Actor. And ensures no floating point precision issues
  // where the Actor moves ever-so-slightly due to near-zero values after the
  // math below.
  if (0.0 == maximumSpeed || (theVector.x() == 0.0 && theVector.y() == 0.0))
  {
    return Eigen::Vector2f(0, 0);
  }

  maximumSpeed *= theIntervalInSeconds;

  if (maximumSpeed > 0.0)
  {
    float vectorMetersPerSecond = theVector.norm();

    if (vectorMetersPerSecond > maximumSpeed)
    {
      float speedDifference = vectorMetersPerSecond - maximumSpeed;
      float speedAdjust = (1.0 - speedDifference/theVector.norm());
      adjustedVector *= speedAdjust;
    }
  }

  // Don't use member in case derived class overrides getMaximumRotationSpeed
  float maximumRotation = getMaximumRotationSpeed() * theIntervalInSeconds;

  // Not using FLOAT_TOLERANCE here in cause user wants a very, very small
  // rotation. Also, making sure the adjusted vector isn't 0,0 as rotation isn't
  // defined for (0,0).
  if (maximumRotation >= 0.0 &&
      (adjustedVector.x() != 0.0 || adjustedVector.y() != 0.0))
  {
    Eigen::Vector2f normalizedInput = adjustedVector.normalized();

    float vectorRadiansPerSecond =
      std::acos(DEFAULT_ORIENTATION.dot(normalizedInput));
    float angleDiff = vectorRadiansPerSecond - maximumRotation;

    if (angleDiff > FLOAT_TOLERANCE)
    {
      // Too much rotation, need to bring it back.
      float radianCorrection = vectorRadiansPerSecond - maximumRotation;

      // From:
      // http://gamedev.stackexchange.com/questions/45412/understanding-math-used-to-determine-if-vector-is-clockwise-counterclockwise-f
      if (DEFAULT_ORIENTATION.y() * adjustedVector.x() >
          DEFAULT_ORIENTATION.x() * adjustedVector.y())
      {
        // Clockwise, will use radianCorrection as is.
      }
      else
      {
        // Counter-clockwise
        radianCorrection *= -1.0;
      }

      Eigen::Rotation2Df rotation(radianCorrection);
      adjustedVector = rotation * adjustedVector;
    }
  }
  return adjustedVector;
}

Eigen::Vector2f QS::Actor::calculateMotionVector(
  const Sensable &theSensable)
{
  BehaviorSet *behaviorSet = selectBehaviorSet(theSensable);
  return behaviorSet->evaluate(this, theSensable);
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

Eigen::Vector3f QS::Actor::getColor() const noexcept
{
  return myColor;
}

float QS::Actor::getMass() const noexcept
{
  return myMass_grams;
}

float QS::Actor::getMaximumRotationSpeed() const noexcept
{
  return myMaximumRotationSpeed_rs;
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

template<class T>
T QS::Actor::getProperty(
  const std::string &thePropertyName, bool theRequired,
  std::function<T(const std::string&)> theConversionFunction,
  T theDefault) const
{
  T value = theDefault;
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

  myColor = getProperty("color", false, toColor, {1,1,1});
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
  myPosition.x() = getProperty("x", true, toFloat);
  myPosition.y() = getProperty("y", true, toFloat);
}

void QS::Actor::setVelocity(const Eigen::Vector2f &theVelocity) noexcept
{
  myVelocity_ms = theVelocity;
}
