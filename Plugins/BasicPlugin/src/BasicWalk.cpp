/**
 * @file BasicWalk.cpp
 * @brief Definition of BasicWalk
 *
 * @author Michael Albers
 */

#include "BasicWalk.h"

QS::BasicWalk::BasicWalk(const Properties &theProperties) :
  BehaviorSet(theProperties)
{
}

Eigen::Vector2f QS::BasicWalk::evaluate(const Sensable &theSensable)
{
  // TODO: implement this
  return {0,0};
}
