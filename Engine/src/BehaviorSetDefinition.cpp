/**
 * @file BehaviorSetDefinition.cpp
 * @brief Definition of BehaviorSetDefinition
 *
 * @author Michael Albers
 */

#include "BehaviorSetDefinition.h"

QS::BehaviorSetDefinition::BehaviorSetDefinition(
  const std::string &theName,
  const std::vector<std::string> &theBehaviors) :
  myName(theName),
  myBehaviors(theBehaviors)
{
  // myName, myBehaviors or any entry in myBehaviors cannot be empty. However,
  // this is levied upon the plugin configuration reader/schema.
}

std::string QS::BehaviorSetDefinition::getName() const noexcept
{
  return myName;
}

std::vector<std::string> QS::BehaviorSetDefinition::getBehaviors()
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
