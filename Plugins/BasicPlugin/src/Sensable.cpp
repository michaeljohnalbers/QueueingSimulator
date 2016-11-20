/**
 * @file Sensable.cpp
 * @brief Definition of Sensable
 *
 * @author Michael Albers
 */

#include "Sensable.h"

QS::Sensable::Sensable(const Actor *theCurrentActor,
                       const std::vector<const Actor*> &theActors,
                       const std::vector<const Exit*> &theExits,
                       float theIntervalInSeconds) noexcept :
  myActors(theActors),
  myCurrentActor(theCurrentActor),
  myExits(theExits),
  myIntervalInSeconds(theIntervalInSeconds)
{
}

const std::vector<const QS::Actor*>& QS::Sensable::getActors() const noexcept
{
  return myActors;
}

const QS::Actor* QS::Sensable::getCurrentActor() const noexcept
{
  return myCurrentActor;
}

const std::vector<const QS::Exit*>& QS::Sensable::getExits() const noexcept
{
  return myExits;
}

float QS::Sensable::getIntervalInSeconds() const noexcept
{
  return myIntervalInSeconds;
}
