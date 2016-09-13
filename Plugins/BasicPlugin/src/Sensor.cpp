/**
 * @file Sensor.cpp
 * @brief Definition of Sensor
 *
 * @author Michael Albers
 */

#include <stdexcept>
#include "Sensor.h"

QS::Sensor::Sensor(const Properties &theProperties) :
  PluginEntity(theProperties)
{
}
