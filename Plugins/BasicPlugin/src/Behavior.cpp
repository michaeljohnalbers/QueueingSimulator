/**
 * @file Behavior.cpp
 * @brief Definition of Behavior
 *
 * @author Michael Albers
 */

#include "Behavior.h"

QS::Behavior::Behavior(const Properties &theProperties,
                       const std::string &theTag) :
  PluginEntity(theProperties, theTag)
{
}
