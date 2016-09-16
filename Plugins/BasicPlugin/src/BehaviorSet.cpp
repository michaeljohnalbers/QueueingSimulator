/**
 * @file BehaviorSet.cpp
 * @brief Definition of BehaviorSet
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "BehaviorSet.h"

QS::BehaviorSet::BehaviorSet(const Properties &theProperties) :
  PluginEntity(theProperties)
{
}

Eigen::Vector2f QS::BehaviorSet::evaluate(const Sensable &theSensable)
{
  // TODO: implement this
  return {0,0};
}
