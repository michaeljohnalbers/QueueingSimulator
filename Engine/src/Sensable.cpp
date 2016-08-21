/**
 * @file Sensable.cpp
 * @brief Definition of Sensable
 *
 * @author Michael Albers
 */

#include "Sensable.h"

QS::Sensable::Sensable(const std::vector<const Actor*> theActors) noexcept :
  myActors(theActors)
{
}

std::vector<const QS::Actor*> QS::Sensable::getActors() const noexcept
{
  return myActors;
}
