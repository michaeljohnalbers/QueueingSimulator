/**
 * @file BehaviorSet.cpp
 * @brief Definition of BehaviorSet
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "Behavior.h"
#include "BehaviorSet.h"
#include "Sensable.h"

#include <iostream>
#include <iomanip>
#include "EigenHelper.h"

QS::BehaviorSet::BehaviorSet(const Properties &theProperties,
                             const std::string &theTag) :
  PluginEntity(theProperties, theTag)
{
}

Eigen::Vector2f QS::BehaviorSet::evaluate(const Actor *theActor,
                                          const Sensable &theSensable)
{
  Eigen::Vector2f average;
  auto dependencies = getDependencies();
  float count = 0;
  for (auto dependency : dependencies)
  {
    auto motionVector = dependency.myEntity->evaluate(
      theActor, theSensable.getIntervalInSeconds());
    average += motionVector;
    count++;
  }

  average /= count;
  return average;
}
