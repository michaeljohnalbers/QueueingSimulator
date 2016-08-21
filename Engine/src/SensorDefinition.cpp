/**
 * @file SensorDefinition.cpp
 * @brief Definition of SensorDefinition
 *
 * @author Michael Albers
 */

#include "SensorDefinition.h"

QS::SensorDefinition::SensorDefinition(const std::string &theName) :
  myName(theName)
{
  // Plugin definition schema will ensure that the name is not null.
}

std::string QS::SensorDefinition::getName() const noexcept
{
  return myName;
}

bool QS::SensorDefinition::operator==(const SensorDefinition &theOther)
  const noexcept
{
  return (getName() == theOther.getName());
}
