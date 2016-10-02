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

Eigen::Vector2f QS::BehaviorSet::evaluate(const Actor *theActor,
                                          const Sensable &theSensable)
{
  // TODO: average all behavior's vectors
  return {0,0};
}
