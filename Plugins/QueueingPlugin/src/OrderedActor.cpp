/**
 * @file OrderedActor.cpp
 * @brief Definition of OrderedActor
 *
 * @author Michael Albers
 */

#include "OrderedActor.h"
#include "PluginHelper.h"

QS::OrderedActor::OrderedActor(const Properties &theProperties,
                               const std::string &theTag) :
  Actor(theProperties, theTag)
{
  myRank = PluginHelper::getProperty(
    theProperties, "rank", true, PluginHelper::toUint);
}

uint32_t QS::OrderedActor::getRank() const noexcept
{
  return myRank;
}
