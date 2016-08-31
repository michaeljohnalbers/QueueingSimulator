/**
 * @file Plugin.cpp
 * @brief Definition of Plugin
 *
 * @author Michael Albers
 */

#include "Plugin.h"
#include "PluginDefinition.h"

QS::Plugin::Plugin(std::shared_ptr<PluginDefinition> theDefinition) :
  myDefinition(theDefinition)
{
}

QS::Plugin::~Plugin()
{
}
