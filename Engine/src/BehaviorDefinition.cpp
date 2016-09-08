/**
 * @file BehaviorDefinition.cpp
 * @brief Definition of BehaviorDefinition
 *
 * @author Michael Albers
 */

#include "BehaviorDefinition.h"

QS::BehaviorDefinition::BehaviorDefinition(
  const std::string &theName) :
  myName(theName)
{
  // myName, mySensors or any entry in mySensors cannot be empty. However, this
  // is levied upon the plugin configuration reader/schema.
}

void QS::BehaviorDefinition::addSensor(const std::string &theSensor,
                                       const std::string &theSource)  noexcept
{
  mySensors.insert({theSensor, theSource});
}

std::string QS::BehaviorDefinition::getName() const noexcept
{
  return myName;
}

std::set<QS::DefinitionPair> QS::BehaviorDefinition::getSensors()
  const noexcept
{
  return mySensors;
}

bool QS::BehaviorDefinition::operator==(const BehaviorDefinition &theOther)
  const noexcept
{
  return (getName() == theOther.getName() &&
          getSensors() == theOther.getSensors());
}
