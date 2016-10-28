/**
 * @file NullSensor.cpp
 * @brief Definition of NullSensor
 *
 * @author Michael Albers
 */

#include "NullSensor.h"

QS::NullSensor::NullSensor(const Properties &theProperties,
                           const std::string &theTag) :
  Sensor(theProperties, theTag)
{
}

void QS::NullSensor::sense(const Sensable &theSensable)
{
}
