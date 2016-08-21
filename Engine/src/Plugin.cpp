/**
 * @file Plugin.cpp
 * @brief Definition of Plugin
 *
 * @author Michael Albers
 */

#include "Plugin.h"

QS::Plugin::Plugin(const PluginDefinition &theDefinition) :
  myDefinition(theDefinition)
{
}

QS::Plugin::~Plugin()
{
}
