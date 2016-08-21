/**
 * @file ActorDefinition.cpp
 * @brief Definition of ActorDefinition
 *
 * @author Michael Albers
 */

#include "ActorDefinition.h"

QS::ActorDefinition::ActorDefinition(
  const std::string &theName,
  const std::vector<std::string> &theBehaviorSets) :
  myBehaviorSets(theBehaviorSets),
  myName(theName)
{
  // myName, myBehaviorSets or any entry in myBehaviorSets cannot be
  // empty. However, this is levied upon the plugin configuration
  // reader/schema.
}

std::vector<std::string> QS::ActorDefinition::getBehaviorSets() const noexcept
{
  return myBehaviorSets;
}

std::string QS::ActorDefinition::getName() const noexcept
{
  return myName;
}

bool QS::ActorDefinition::operator==(const ActorDefinition &theOther)
  const noexcept
{
  return (getName() == theOther.getName());
}
