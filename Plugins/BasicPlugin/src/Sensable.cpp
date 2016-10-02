/**
 * @file Sensable.cpp
 * @brief Definition of Sensable
 *
 * @author Michael Albers
 */

#include "Sensable.h"

QS::Sensable::Sensable(const std::vector<const Actor*> theActors,
                       const std::chrono::milliseconds &theInterval) noexcept :
  myActors(theActors),
  myInterval(theInterval)
{
}

std::vector<const QS::Actor*> QS::Sensable::getActors() const noexcept
{
  return myActors;
}

std::chrono::milliseconds QS::Sensable::getInterval() const noexcept
{
  return myInterval;
}
