/**
 * @file BehaviorSetDefinition.cpp
 * @brief Definition of BehaviorSetDefinition
 *
 * @author Michael Albers
 */

#include "BehaviorSetDefinition.h"

QS::BehaviorSetDefinition::BehaviorSetDefinition(
  const std::string &theName) :
  myName(theName)
{
  // myName, myBehaviors or any entry in myBehaviors cannot be empty. However,
  // this is levied upon the plugin configuration reader/schema.
}

void QS::BehaviorSetDefinition::addBehavior(const std::string &theBehavior,
                                            const std::string &theSource,
                                            const std::string &theTag)
  noexcept
{
  myBehaviors.insert({theBehavior, theSource, theTag});
}

std::string QS::BehaviorSetDefinition::getName() const noexcept
{
  return myName;
}

std::set<QS::PluginDependencySet> QS::BehaviorSetDefinition::getBehaviors()
  const noexcept
{
  return myBehaviors;
}

bool QS::BehaviorSetDefinition::operator==(
  const BehaviorSetDefinition &theOther) const noexcept
{
  return (getName() == theOther.getName() &&
          getBehaviors() == theOther.getBehaviors());
}
