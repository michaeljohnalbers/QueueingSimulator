/**
 * @file NearestN.cpp
 * @brief Definition of NearestN
 *
 * @author Michael Albers
 */

#include <algorithm>
#include <stdexcept>
#include "Eigen/Core"
#include "Actor.h"
#include "NearestN.h"
#include "PluginHelper.h"
#include "Sensable.h"

QS::NearestN::NearestN(const Properties &theProperties,
                       const std::string &theTag) :
  Sensor(theProperties, theTag)
{
  myN = PluginHelper::getProperty(
    theProperties, "N", true, PluginHelper::toUint);

  myRadius_m = PluginHelper::getProperty(
    theProperties, "radius", true, PluginHelper::toFloat);
  if (myRadius_m < 0.0)
  {
    throw std::invalid_argument(
      "Invalid radius " + std::to_string(myRadius_m) +
      ", must be >= 0 in NearestN constructor.");
  }
}

const std::vector<const QS::Actor*>& QS::NearestN::getActors() const noexcept
{
  return myActors;
}

uint32_t QS::NearestN::getN() const noexcept
{
  return myN;
}

float QS::NearestN::getRadius() const noexcept
{
  return myRadius_m;
}

void QS::NearestN::sense(const Sensable &theSensable)
{
  myActors = theSensable.getActors();
  const Eigen::Vector2f currentActorPosition =
    theSensable.getCurrentActor()->getPosition();
  auto sort = [&](const Actor *a, const Actor *b) ->bool
  {
    float distanceA = (a->getPosition() - currentActorPosition).norm();
    float distanceB = (b->getPosition() - currentActorPosition).norm();
    return distanceA < distanceB;
  };
  std::sort(myActors.begin(), myActors.end(), sort);

  if (myActors.size() > myN)
  {
    myActors.resize(myN);
  }

  auto actorIter = myActors.begin();
  while (actorIter != myActors.end())
  {
    float distance =
      ((*actorIter)->getPosition() - currentActorPosition).norm();
    if (distance > myRadius_m)
    {
      actorIter = myActors.erase(actorIter);
    }
    else
    {
      ++actorIter;
    }
  }
}
