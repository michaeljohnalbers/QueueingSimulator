/**
 * @file Walk.cpp
 * @brief Definition of Walk
 *
 * @author Michael Albers
 */

#include "Walk.h"

QS::Walk::Walk(const Properties &theProperties) :
  Behavior(theProperties)
{
}

Eigen::Vector2f QS::Walk::evaluate()
{
  // TODO: implement
  return {0,0};
}
