/**
 * @file PluginDependencySet.cpp
 * @brief Definition of PluginDependencySet
 *
 * @author Michael Albers
 */

#include "PluginDependencySet.h"

bool QS::PluginDependencySet::operator==(const PluginDependencySet &theOther)
  const noexcept
{
  return (myName == theOther.myName && mySource == theOther.mySource &&
          myTag == theOther.myTag);
}

bool QS::PluginDependencySet::operator<(const PluginDependencySet &theOther)
  const noexcept
{
  return (myName < theOther.myName || myTag < theOther.myTag ||
          mySource < theOther.mySource);
}
