/**
 * @file ExitDefinition.cpp
 * @brief Definition of ExitDefinition
 *
 * @author Michael Albers
 */

#include "ExitDefinition.h"

QS::ExitDefinition::ExitDefinition(const std::string &theName) :
  myName(theName)
{
  // Plugin definition schema will ensure that the name is not null.
}

std::string QS::ExitDefinition::getName() const noexcept
{
  return myName;
}

bool QS::ExitDefinition::operator==(const ExitDefinition &theOther)
  const noexcept
{
  return (getName() == theOther.getName());
}
