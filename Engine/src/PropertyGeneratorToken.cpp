/**
 * @file PropertyGeneratorToken.cpp
 * @brief Implementation of PropertyGeneratorToken
 *
 * @author Michael Albers
 */

#include "PropertyGeneratorToken.h"

// Static Variables
QS::PropertyGeneratorToken::TokenMap
QS::PropertyGeneratorToken::ourTokenDescriptions{
  {Type::RandSym, "Rand"},
  {Type::RandColorSym, "RandColor"},
  {Type::Id, "Id"},
  {Type::IntLiteral, "IntLiteral"},
  {Type::FloatLiteral, "IntLiteral"},
  {Type::LParen, "LParen"},
  {Type::RParen, "RParen"},
  {Type::Comma, "Comma"},
  {Type::PlusOp, "PlusOp"},
  {Type::MinusOp, "MinusOp"},
  {Type::MultOp, "MultOp"},
  {Type::DivideOp, "DivideOp"},
  {Type::EofSym, "EofSym"},
};

QS::PropertyGeneratorToken::PropertyGeneratorToken(Type theToken,
                                                   uint32_t theIndex) :
  PropertyGeneratorToken(theToken, "", theIndex)
{
}

QS::PropertyGeneratorToken::PropertyGeneratorToken(
  Type theToken,
  const std::string &theTokenLiteral,
  uint32_t theIndex) :
  myIndex(theIndex),
  myToken(theToken),
  myTokenLiteral(theTokenLiteral)
{
}

uint32_t QS::PropertyGeneratorToken::getIndex() const noexcept
{
  return myIndex;
}

std::string QS::PropertyGeneratorToken::getLiteral() const noexcept
{
  return myTokenLiteral;
}

QS::PropertyGeneratorToken::Type QS::PropertyGeneratorToken::getToken()
  const noexcept
{
  return myToken;
}

std::string QS::PropertyGeneratorToken::getTokenString() const noexcept
{
  return ourTokenDescriptions[getToken()];
}

bool QS::PropertyGeneratorToken::operator<(
  const PropertyGeneratorToken &theRHS) const noexcept
{
  return getToken() < theRHS.getToken();
}

bool QS::PropertyGeneratorToken::operator==(
  const PropertyGeneratorToken &theRHS) const noexcept
{
  return getToken() == theRHS.getToken();
}

namespace QS
{
std::ostream& operator<<(std::ostream &theOS,
                         const QS::PropertyGeneratorToken &theToken)
{
  theOS << theToken.getTokenString()
        << " (\"" << theToken.getLiteral() << "\")";
  return theOS;
}
}
