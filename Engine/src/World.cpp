/**
 * @file World.cpp
 * @brief Definition of World
 *
 * @author Michael Albers
 */

#include "World.h"

void QS::World::setDimentions(float theWidth_m, float theLength_m)
{
  myWidth_m = theWidth_m;
  myLength_m = theLength_m;
}

void QS::World::setSeed(uint64_t theSeed)
{
  myRNGEngine.seed(theSeed);
}
