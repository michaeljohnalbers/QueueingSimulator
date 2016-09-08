#pragma once

/**
 * @file DefinitionPair.h
 * @brief Simple class to encapsulate name/source pair for definition classes.
 *
 * @author Michael Albers
 */

#include <string>

namespace QS
{
  /**
   * Small helper class just to encapsulate name/source information.
   */
  class DefinitionPair
  {
    public:
    std::string myName;
    std::string mySource;
    bool operator==(const DefinitionPair &theRHS) const noexcept;
    bool operator<(const DefinitionPair &theRHS) const noexcept;
  };
};
