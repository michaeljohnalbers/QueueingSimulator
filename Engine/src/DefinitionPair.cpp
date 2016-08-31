/**
 * @file DefinitionPair.cpp
 * @brief Definition of DefinitionPair
 *
 * @author Michael Albers
 */

#include "DefinitionPair.h"

bool QS::DefinitionPair::operator==(const DefinitionPair &theOther)
  const noexcept
{
  return (myName == theOther.myName && mySource == theOther.mySource);
}

bool QS::DefinitionPair::operator<(const DefinitionPair &theOther)
  const noexcept
{
  return (myName < theOther.myName || mySource < theOther.mySource);
}
