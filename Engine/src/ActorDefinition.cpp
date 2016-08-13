/**
 * @file ActorDefinition.cpp
 * @brief Definition of ActorDefinition
 *
 * @author Michael Albers
 */

#include "ActorDefinition.h"

QS::ActorDefinition::ActorDefinition(
  const std::string &theName,
  const std::vector<std::string> &theBehaviors) :
  myBehaviorSets(theBehaviors),
  myName(theName)
{
}
