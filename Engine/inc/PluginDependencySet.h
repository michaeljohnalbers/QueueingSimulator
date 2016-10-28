#pragma once

/**
 * @file PluginDependencySet.h
 * @brief Simple class to encapsulate name/source/tag set for definition
 * classes.
 *
 * @author Michael Albers
 */

#include <string>

namespace QS
{
  /**
   * Small helper class just to encapsulate name/source/tag information.
   */
  class PluginDependencySet
  {
    public:
    std::string myName;
    std::string mySource;
    std::string myTag;
    bool operator==(const PluginDependencySet &theRHS) const noexcept;
    bool operator<(const PluginDependencySet &theRHS) const noexcept;
  };
};
