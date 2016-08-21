/**
 * @file Sensor.cpp
 * @brief Definition of Sensor
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "Sensor.h"

QS::Sensor::Sensor(const std::string &theTypeName) :
  myTypeName(theTypeName)
{
  if (myTypeName.size() == 0)
  {
    throw std::invalid_argument("Sensor type name cannot be empty "
                                "in Sensor constructor.");
  }
}

std::string QS::Sensor::getTypeName() const noexcept
{
  return myTypeName;
}
