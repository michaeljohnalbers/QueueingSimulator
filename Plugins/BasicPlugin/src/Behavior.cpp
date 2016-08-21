/**
 * @file Behavior.cpp
 * @brief Definition of Behavior
 *
 * @author Michael Albers
 */

#include "Behavior.h"

QS::Behavior::Behavior(const std::vector<std::string> &theInputSensorTypes) :
  myInputSensorTypes(theInputSensorTypes)
{
}

std::vector<std::string> QS::Behavior::getInputSensorTypes() const noexcept
{
  return myInputSensorTypes;
}
