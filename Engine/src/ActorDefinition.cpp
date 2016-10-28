/**
 * @file ActorDefinition.cpp
 * @brief Definition of ActorDefinition
 *
 * @author Michael Albers
 */

#include "ActorDefinition.h"

QS::ActorDefinition::ActorDefinition(const std::string &theName) :
  myName(theName)
{
  // myName, myBehaviorSets or any entry in myBehaviorSets cannot be
  // empty. However, this is levied upon the plugin configuration
  // reader/schema.
}

void QS::ActorDefinition::addBehaviorSet(const std::string &theBehaviorSet,
                                         const std::string &theSource,
                                         const std::string &theTag) noexcept
{
  myBehaviorSets.insert({theBehaviorSet, theSource, theTag});
}

std::set<QS::PluginDependencySet> QS::ActorDefinition::getBehaviorSets()
  const noexcept
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
