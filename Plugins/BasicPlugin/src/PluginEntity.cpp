/**
 * @file PluginEntity.cpp
 * @brief Definition of PluginEntity
 *
 * @author Michael Albers
 */

#include "PluginEntity.h"

QS::PluginEntity::PluginEntity(const Properties &theProperties) :
  myProperties(theProperties)
{
}

QS::PluginEntity::Properties QS::PluginEntity::getProperties() const noexcept
{
  return myProperties;
}
