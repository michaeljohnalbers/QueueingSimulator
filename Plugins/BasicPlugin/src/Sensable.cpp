/**
 * @file Sensable.cpp
 * @brief Definition of Sensable
 *
 * @author Michael Albers
 */

#include "Sensable.h"

QS::Sensable::Sensable(const std::vector<const Actor*> theActors,
                       float theIntervalInSeconds) noexcept :
  myActors(theActors),
  myIntervalInSeconds(theIntervalInSeconds)
{
}

std::vector<const QS::Actor*> QS::Sensable::getActors() const noexcept
{
  return myActors;
}

float QS::Sensable::getIntervalInSeconds() const noexcept
{
  return myIntervalInSeconds;
}
