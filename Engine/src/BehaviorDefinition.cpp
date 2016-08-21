/**
 * @file BehaviorDefinition.cpp
 * @brief Definition of BehaviorDefinition
 *
 * @author Michael Albers
 */

#include "BehaviorDefinition.h"

QS::BehaviorDefinition::BehaviorDefinition(
  const std::string &theName,
  const std::vector<std::string> &theSensors) :
  myName(theName),
  mySensors(theSensors)
{
  // myName, mySensors or any entry in mySensors cannot be empty. However, this
  // is levied upon the plugin configuration reader/schema.
}

std::string QS::BehaviorDefinition::getName() const noexcept
{
  return myName;
}

std::vector<std::string> QS::BehaviorDefinition::getSensors() const noexcept
{
  return mySensors;
}

bool QS::BehaviorDefinition::operator==(const BehaviorDefinition &theOther)
  const noexcept
{
  return (getName() == theOther.getName() &&
          getSensors() == theOther.getSensors());
}
