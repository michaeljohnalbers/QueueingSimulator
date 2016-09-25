/**
 * @file World.cpp
 * @brief Definition of World
 *
 * @author Michael Albers
 */

#include "World.h"

void QS::World::addActor(Actor *theActor)
{
  // TODO: need to verify this Actor does not overlap with any other Actor, and
  // that it is fully within the world bounds
  myActors.push_back(theActor);
}

const std::vector<QS::Actor*>& QS::World::getActors() const noexcept
{
  return myActors;
}

std::tuple<float, float> QS::World::getDimensions() const noexcept
{
  return std::make_tuple(myWidth_m, myLength_m);
}

void QS::World::setDimentions(float theWidth_m, float theLength_m)
{
  myWidth_m = theWidth_m;
  myLength_m = theLength_m;
}

void QS::World::setSeed(uint64_t theSeed)
{
  myRNGEngine.seed(theSeed);
}
