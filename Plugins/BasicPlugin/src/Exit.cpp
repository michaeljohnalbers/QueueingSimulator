/**
 * @file Exit.cpp
 * @brief Definition of Exit
 *
 * @author Michael Albers
 */

#include <cmath>
#include <sstream>
#include "Actor.h"
#include "Exit.h"
#include "PluginHelper.h"

QS::Exit::Exit(const Properties &theProperties, const std::string &theTag) :
  Target(theProperties, theTag),
  myColor(1.0, 1.0, 1.0) // Use white as OpenGL clear color is black
{
  float radius = getRadius();
  if (radius == 0)
  {
    std::ostringstream error;
    error << "Invalid Exit radius, " << std::fixed
          << radius << ", it must be greater than zero.";
    throw std::invalid_argument(error.str());
  }

  setColorFromProperty();
}

bool QS::Exit::canActorExit(const Actor *theActor) noexcept
{
  auto exitPosition = getPosition();
  auto actorPosition = theActor->getPosition();

  // From:
  // http://math.stackexchange.com/questions/198764/how-to-know-if-a-point-is-inside-a-circle
  float xDiff = actorPosition.x() - exitPosition.x();
  float yDiff = actorPosition.y() - exitPosition.y();
  float d = std::sqrt((xDiff * xDiff) + (yDiff * yDiff));

  bool actorInsideExit = (d < getRadius());
  return actorInsideExit;
}

Eigen::Vector3f QS::Exit::getColor() const noexcept
{
  return myColor;
}

void QS::Exit::setColorFromProperty()
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
            "Exit constructor.";
          throw std::invalid_argument(error);
        }
        color(ii) = tempColor;
      }
      return color;
    };

  myColor = PluginHelper::getProperty(
    myProperties, "color", false, toColor, {1,1,1});
}

void QS::Exit::update(float theIntervalInSeconds)
{
  // Nothing to do in this definition.
}
