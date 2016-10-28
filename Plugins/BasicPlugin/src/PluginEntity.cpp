/**
 * @file PluginEntity.cpp
 * @brief Definition of PluginEntity
 *
 * @author Michael Albers
 */

#include "PluginEntity.h"

QS::PluginEntity::PluginEntity(const Properties &theProperties,
                               const std::string &theTag) :
  myProperties(theProperties),
  myTag(theTag)
{
}

QS::PluginEntity::Properties QS::PluginEntity::getProperties() const noexcept
{
  return myProperties;
}

std::string QS::PluginEntity::getTag() const noexcept
{
  return myTag;
}
