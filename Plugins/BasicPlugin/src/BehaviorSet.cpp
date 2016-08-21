/**
 * @file BehaviorSet.cpp
 * @brief Definition of BehaviorSet
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "BehaviorSet.h"

QS::BehaviorSet::BehaviorSet(
  const std::map<std::string, Behavior*> &theBehaviors) :
  myBehaviors(theBehaviors)
{
  if (myBehaviors.size() == 0)
  {
    throw std::invalid_argument("Cannot have empty set of behaviors in "
                                "BehaviorSet constructor.");
  }

  for (auto behavior : theBehaviors)
  {
    // For the time being, having an empty name is OK. The name is really just
    // a key for the BehaviorSet author and if they don't want it, I guess
    // that's their business.

    if (nullptr == behavior.second)
    {
      throw std::invalid_argument("Cannot have null Behavior (for name '" +
                                  behavior.first +
                                  "') in BehaviorSet constructor.");
    }
  }
}

Eigen::Vector2f QS::BehaviorSet::evaluate(const Sensable &theSensable)
{
  // TODO: implement this
  return {0,0};
}
