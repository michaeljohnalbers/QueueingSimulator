/**
 * @file PropertyGeneratorScanner.cpp
 * @brief Implementation of PropertyGeneratorScanner class
 *
 * @author Michael Albers
 */

#include <algorithm>
#include <cctype>
#include <cstring>
#include <stdexcept>

#include "PropertyGeneratorScanner.h"

#include <iostream>

// Static Variables
QS::PropertyGeneratorScanner::ReservedWordMap
QS::PropertyGeneratorScanner::ourReservedWords{
  {"rand", PropertyGeneratorToken::Type::RandSym},
};


QS::PropertyGeneratorScanner::PropertyGeneratorScanner(
  const std::string &theInput) :
  myIndex(0),
  myInput(theInput)
{
}

void QS::PropertyGeneratorScanner::advance()
{
  ++myIndex;
}

QS::PropertyGeneratorToken QS::PropertyGeneratorScanner::getCurrentToken() const
{
  return myCurrentToken;
}

QS::PropertyGeneratorToken QS::PropertyGeneratorScanner::idOrReserved(
  const std::string &theTokenLiteral,
  uint32_t theTokenStartIndex)
{
  std::string token {theTokenLiteral};
  std::transform(token.begin(), token.end(), token.begin(), ::tolower);

  PropertyGeneratorToken idOrReserved(
    PropertyGeneratorToken::Type::Id, theTokenLiteral, theTokenStartIndex);

  auto reservedIter = ourReservedWords.find(token);
  if (reservedIter != ourReservedWords.end())
  {
    idOrReserved = PropertyGeneratorToken(
      reservedIter->second, theTokenLiteral, theTokenStartIndex);
  }

  return idOrReserved;
}

char QS::PropertyGeneratorScanner::inspectCharacter()
{
  // Last iteration through the input has myIndex == myInput.size(), this
  // prevents an unitialized memory read.
  char c = 0;
  if (myIndex < myInput.size())
  {
    c = myInput[myIndex];
  }
  return c;
}

void QS::PropertyGeneratorScanner::newLine()
{
  // Nothing to do at this time.
}

QS::PropertyGeneratorToken QS::PropertyGeneratorScanner::nextToken()
{
  myCurrentToken = peek();
  myPeekToken = readToken();
  return myCurrentToken;
}

QS::PropertyGeneratorToken QS::PropertyGeneratorScanner::peek()
{
  if (nullptr == myPeekTokenPtr)
  {
    myPeekToken = readToken();
    myPeekTokenPtr = &myPeekToken;
  }
  return *myPeekTokenPtr;
}

char QS::PropertyGeneratorScanner::readCharacter()
{
  auto character = inspectCharacter();
  advance();
  return character;
}

QS::PropertyGeneratorToken QS::PropertyGeneratorScanner::readToken()
{
  std::string thisBuffer;

  auto addCharacterToBuffer = [&](char theCharacter)
  {
    thisBuffer += theCharacter;
  };

  PropertyGeneratorToken nextToken;

  while (true)
  {
    char currentChar = readCharacter();
    if (myIndex <= myInput.size())
    {
      // Start of (potential) new token

      uint32_t tokenStartIndex = myIndex;

      if (std::isspace(currentChar))
      {
        if ('\n' == currentChar)
        {
          newLine();
        }
      }
      else if (std::isalpha(currentChar))
      {
        addCharacterToBuffer(currentChar);
        while (true)
        {
          currentChar = inspectCharacter();
          if (std::isalnum(currentChar) || '_' == currentChar ||
              '.' == currentChar)
          {
            addCharacterToBuffer(currentChar);
            advance();
          }
          else
          {
            nextToken = idOrReserved(thisBuffer, tokenStartIndex);
            auto tokenLength = nextToken.getLiteral().size();
            if (PropertyGeneratorToken::Type::Id == nextToken.getToken() &&
                tokenLength > MAX_ID_LENGTH)
            {
              syntaxError("Invalid length of " +
                          std::to_string(tokenLength) +
                          " characters for identifier '" +
                          nextToken.getLiteral() +
                          "'. Identifiers can be at most " +
                          std::to_string(MAX_ID_LENGTH) + " characters.");
            }

            return nextToken;
          }
        }
      }
      else if (std::isdigit(currentChar))
      {
        bool intLiteral = true;
        addCharacterToBuffer(currentChar);
        while (true)
        {
          currentChar = inspectCharacter();
          if (std::isdigit(currentChar))
          {
            addCharacterToBuffer(currentChar);
            advance();
          }
          else if ('.' == currentChar && intLiteral)
          {
            intLiteral = false;
            addCharacterToBuffer(currentChar);
            advance();
          }
          else
          {
            if (intLiteral)
            {
              nextToken = PropertyGeneratorToken(
                PropertyGeneratorToken::Type::IntLiteral, thisBuffer,
                tokenStartIndex);
            }
            else
            {
              nextToken = PropertyGeneratorToken(
                PropertyGeneratorToken::Type::FloatLiteral, thisBuffer,
                tokenStartIndex);
            }
            return nextToken;
          }
        }
      }
      else if ('(' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = PropertyGeneratorToken(
          PropertyGeneratorToken::Type::LParen, thisBuffer, tokenStartIndex);
        return nextToken;
      }
      else if (')' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = PropertyGeneratorToken(
          PropertyGeneratorToken::Type::RParen, thisBuffer, tokenStartIndex);
        return nextToken;
      }
      else if (',' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = PropertyGeneratorToken(
          PropertyGeneratorToken::Type::Comma, thisBuffer, tokenStartIndex);
        return nextToken;
      }
      else if ('+' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = PropertyGeneratorToken(
          PropertyGeneratorToken::Type::PlusOp, thisBuffer, tokenStartIndex);
        return nextToken;
      }
      else if ('*' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = PropertyGeneratorToken(
          PropertyGeneratorToken::Type::MultOp, thisBuffer, tokenStartIndex);
        return nextToken;
      }
      else if ('/' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = PropertyGeneratorToken(
          PropertyGeneratorToken::Type::DivideOp, thisBuffer, tokenStartIndex);
        return nextToken;
      }
      else if ('-' == currentChar)
      {
        addCharacterToBuffer(currentChar);
        nextToken = PropertyGeneratorToken(
          PropertyGeneratorToken::Type::MinusOp, thisBuffer,tokenStartIndex);
        return nextToken;
      }
      else
      {
        std::string error{"Read unexpected character '"};
        error += currentChar + std::string("' (ASCII decimal ") +
          std::to_string(currentChar) + ").";
        syntaxError(error);
      }
    }
    else
    {
      nextToken = PropertyGeneratorToken(
        PropertyGeneratorToken::Type::EofSym, 0);
      break;
    }
  }
  return nextToken;
}

void QS::PropertyGeneratorScanner::syntaxError(const std::string &theError)
{
  std::string error;
  error += "At index" + std::to_string(myIndex) + ": error: " + theError;
  throw std::runtime_error{error};
}
