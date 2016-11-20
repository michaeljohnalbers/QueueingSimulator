/**
 * @file FindExitSensor.cpp
 * @brief Definition of FindExitSensor
 *
 * @author Michael Albers
 */

#include "FindExitSensor.h"
#include "Sensable.h"

QS::FindExitSensor::FindExitSensor(const Properties &theProperties,
                                   const std::string &theTag) :
  Sensor(theProperties, theTag)
{
}

const QS::Exit* QS::FindExitSensor::getExit() const noexcept
{
  return myExit;
}

void QS::FindExitSensor::sense(const Sensable &theSensable)
{
  myExit = nullptr;
  auto &exits = theSensable.getExits();
  if (! exits.empty())
  {
    myExit = exits.front();
  }
}
