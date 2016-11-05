/**
 * @file PropertyGenerator.cpp
 * @brief Definition of PropertyGenerator class
 *
 * @author Michael Albers
 */

#define _USE_MATH_DEFINES // For M_PI
#include <cmath>
#include <random>
#include <sstream>
#include <strings.h>
#include "SimulationEntityConfiguration.h"
#include "PropertyGenerator.h"
#include "World.h"

QS::PropertyGenerator::PropertyGenerator(World &theWorld) :
  myWorld(theWorld)
{
}

void QS::PropertyGenerator::addOp(OperatorRecord &theOperator)
{
  checkInput({PropertyGeneratorToken::Type::PlusOp,
              PropertyGeneratorToken::Type::MinusOp});

  PropertyGeneratorToken peekToken(myScanner->peek());
  switch (peekToken.getToken())
  {
    case PropertyGeneratorToken::Type::PlusOp:
      match(PropertyGeneratorToken::Type::PlusOp);
      theOperator = OperatorRecord(PropertyGeneratorToken::Type::PlusOp);
      break;

    case PropertyGeneratorToken::Type::MinusOp:
      match(PropertyGeneratorToken::Type::MinusOp);
      theOperator = OperatorRecord(PropertyGeneratorToken::Type::MinusOp);
      break;

    default:
      // Won't get here due to checkInput above, but g++ generates warnings
      // for unhandled enums.
      break;
  }
}

void QS::PropertyGenerator::checkInput(const TokenSet &theValidSet)
{
  PropertyGeneratorToken nextToken{myScanner->peek()};
  if (theValidSet.find(nextToken.getToken()) == theValidSet.end())
  {
    std::ostringstream errorStr;
    errorStr << "Recieved token " << nextToken << ". Expected one of: ";
    bool first = true;
    for (auto validToken : theValidSet)
    {
      if (!first)
      {
        errorStr << " ";
      }
      first = false;
      errorStr << PropertyGeneratorToken(validToken, 0).getTokenString();
    }

    error(nextToken, errorStr.str());
  }
}

void QS::PropertyGenerator::error(const PropertyGeneratorToken &theToken,
                                  const std::string &theError)
{
  std::ostringstream errorText;
  errorText << "Error generating property from \"" << myOriginalProperty
            << "\". Error at index " << theToken.getIndex() << ". " << theError;
  throw std::runtime_error(errorText.str());
}

void QS::PropertyGenerator::expression()
{
  checkInput({PropertyGeneratorToken::Type::FloatLiteral,
              PropertyGeneratorToken::Type::Id,
              PropertyGeneratorToken::Type::IntLiteral,
              PropertyGeneratorToken::Type::LParen});

  term();
  expressionTail();
}

void QS::PropertyGenerator::expressionTail()
{
  PropertyGeneratorToken peekToken(myScanner->peek());
  if (peekToken.getToken() == PropertyGeneratorToken::Type::PlusOp ||
      peekToken.getToken() == PropertyGeneratorToken::Type::MinusOp)
  {
    OperatorRecord operatorRecord(PropertyGeneratorToken::Type::PlusOp);
    addOp(operatorRecord);

    term();

    auto right = myEvaluationStack.top();
    myEvaluationStack.pop();
    auto left = myEvaluationStack.top();
    myEvaluationStack.pop();

    if (operatorRecord.getToken() == PropertyGeneratorToken::Type::PlusOp)
    {
      float result = left + right;
      myEvaluationStack.push(result);
    }
    else
    {
      float result = left - right;
      myEvaluationStack.push(result);
    }

    expressionTail();
  }
}

void QS::PropertyGenerator::ident()
{
  checkInput({PropertyGeneratorToken::Type::Id});

  match(PropertyGeneratorToken::Type::Id);

  auto ident = myScanner->getCurrentToken().getLiteral();

  if (::strcasecmp(ident.c_str(), "PI") == 0)
  {
    myEvaluationStack.push(M_PI);
  }
  else if (::strncasecmp(ident.c_str(), "world", 5) == 0)
  {
    auto dotPosition = ident.find(".");
    if (dotPosition != std::string::npos)
    {
      auto identName = ident.substr(0, dotPosition);
      auto identAttributeName = ident.substr(dotPosition + 1);
      if (::strncasecmp(identName.c_str(), "world", dotPosition) != 0)
      {
        // Hack here, real scanner/parser would handle world.length as multiple
        // tokens.
        std::string error{"Unknown identifier \""};
        error += identName + "\".";
        throw std::runtime_error(error);
      }

      if (::strcasecmp(identAttributeName.c_str(), "width") == 0)
      {
        myEvaluationStack.push(std::get<0>(myWorld.getDimensions()));
      }
      else if (::strcasecmp(identAttributeName.c_str(), "length") == 0)
      {
        myEvaluationStack.push(std::get<1>(myWorld.getDimensions()));
      }
      else
      {
        std::string error{"Invalid world attribute provided, \""};
        error += identAttributeName + "\". Expected \"width\" or \"length\".";
        throw std::runtime_error(error);
      }
    }
    else
    {
      std::string error{"No world attribute provided."};
      error += "  Expected \"width\" or \"length\".";
      throw std::runtime_error(error);
    }
  }
  else if (::strncasecmp(ident.c_str(), "this", 4) == 0)
  {
    auto dotPosition = ident.find(".");
    if (dotPosition != std::string::npos)
    {
      auto identName = ident.substr(0, dotPosition);
      auto identAttributeName = ident.substr(dotPosition + 1);

      if (::strncasecmp(identName.c_str(), "this", dotPosition) != 0)
      {
        // Hack here, real scanner/parser would handle world.length as multiple
        // tokens.
        std::string error{"Unknown identifier \""};
        error += identName + "\".";
        throw std::runtime_error(error);
      }

      auto properties = myEntity->getProperties();
      try
      {
        auto property = properties.at(identAttributeName);

        try
        {
          myEvaluationStack.push(std::stof(property));
        }
        catch (std::logic_error)
        {
          // Handles out_of_range & invalid_argument that stof can throw.
          std::string error{"Invalid entity property value for property, \""};
          error += identAttributeName +
            "\". Value must be convertable to a float.";
          throw std::runtime_error(error);
        }
      }
      catch (std::out_of_range)
      {
        std::string error{"Invalid entity property provided, \""};
        error += identAttributeName +
          "\". (It might not have been processed yet.)";
        throw std::runtime_error(error);
      }
    }
    else
    {
      std::string error{"No entity attribute provided."};
      throw std::runtime_error(error);
    }
  }
  else
  {
    std::string error{"Unknown identifier \""};
    error += ident + "\".";
    throw std::runtime_error(error);
  }
}

std::string QS::PropertyGenerator::generateProperty(
  const std::string &theValue,
  const SimulationEntityConfiguration &theCurrentEntity)
{
  myEntity = &theCurrentEntity;
  std::string newValue{theValue};
  if (theValue[0] == ':')
  {
    myOriginalProperty = theValue;
    newValue.erase(0, 1); // Erase the ':'
    myScanner.reset(new PropertyGeneratorScanner(newValue));

    // std::stack has no clear function.
    while (! myEvaluationStack.empty())
    {
      myEvaluationStack.pop();
    }

    parse();
    auto result = myEvaluationStack.top();
    if (StackItem::Type::Float == result.myType)
    {
      newValue = std::to_string(result.myFloat);
    }
    else
    {
      newValue = result.myString;
    }
    myEvaluationStack.pop();
  }

  return newValue;
}

void QS::PropertyGenerator::match(const PropertyGeneratorToken::Type &theToken)
{
  PropertyGeneratorToken peekToken(myScanner->peek());
  if (theToken == peekToken.getToken())
  {
    myScanner->nextToken();
  }
  else
  {
    std::ostringstream errorStr;
    errorStr << "Expected " << PropertyGeneratorToken(theToken, 0)
             << ". Instead found " << peekToken << ".";
    error(peekToken, errorStr.str());
  }
}

void QS::PropertyGenerator::multOp(OperatorRecord &theOperator)
{
  checkInput({PropertyGeneratorToken::Type::MultOp,
              PropertyGeneratorToken::Type::DivideOp});

  PropertyGeneratorToken peekToken(myScanner->peek());
  switch (peekToken.getToken())
  {
    case PropertyGeneratorToken::Type::MultOp:
      match(PropertyGeneratorToken::Type::MultOp);
      theOperator = OperatorRecord(PropertyGeneratorToken::Type::MultOp);
      break;

    case PropertyGeneratorToken::Type::DivideOp:
      match(PropertyGeneratorToken::Type::DivideOp);
      theOperator = OperatorRecord(PropertyGeneratorToken::Type::DivideOp);
      break;

    default:
      // Won't get here due to checkInput above, but g++ generates warnings
      // for unhandled enums.
      break;
  }
}

void QS::PropertyGenerator::parse()
{
  statement();
  match(PropertyGeneratorToken::Type::EofSym);
}

void QS::PropertyGenerator::primary()
{
  checkInput({PropertyGeneratorToken::Type::FloatLiteral,
              PropertyGeneratorToken::Type::Id,
              PropertyGeneratorToken::Type::IntLiteral,
              PropertyGeneratorToken::Type::LParen});

  PropertyGeneratorToken peekToken(myScanner->peek());
  switch (peekToken.getToken())
  {
    case PropertyGeneratorToken::Type::FloatLiteral:
    {
      match(PropertyGeneratorToken::Type::FloatLiteral);
      auto literal = myScanner->getCurrentToken().getLiteral();
      myEvaluationStack.push(std::stof(literal));
    }
    break;

    case PropertyGeneratorToken::Type::Id:
      ident();
      break;

    case PropertyGeneratorToken::Type::IntLiteral:
    {
      match(PropertyGeneratorToken::Type::IntLiteral);
      auto literal = myScanner->getCurrentToken().getLiteral();
      myEvaluationStack.push(std::stof(literal));
    }
    break;

    case PropertyGeneratorToken::Type::LParen:
    {
      match(PropertyGeneratorToken::Type::LParen);
      expression();
      match(PropertyGeneratorToken::Type::RParen);
    }
    break;

    default:
      // Won't get here due to checkInput above, but g++ generates warnings
      // for unhandled enums.
      break;
  }
}

void QS::PropertyGenerator::rand()
{
  checkInput({PropertyGeneratorToken::Type::RandSym});

  match(PropertyGeneratorToken::Type::RandSym);
  match(PropertyGeneratorToken::Type::LParen);
  expression();
  float lowBound = myEvaluationStack.top();
  myEvaluationStack.pop();
  match(PropertyGeneratorToken::Type::Comma);
  expression();
  float highBound = myEvaluationStack.top();
  myEvaluationStack.pop();
  match(PropertyGeneratorToken::Type::RParen);

  if (lowBound > highBound)
  {
    std::string error{"Invalid random bounds. Low is larger than high. Low: "};
    error += std::to_string(lowBound) + ", high: " + std::to_string(highBound)
      + ".";
    throw std::runtime_error(error);
  }

  std::uniform_real_distribution<float> distribution(lowBound, highBound);

  myEvaluationStack.push(myWorld.getRandomNumber(distribution));
}

void QS::PropertyGenerator::randColor()
{
  checkInput({PropertyGeneratorToken::Type::RandColorSym});

  match(PropertyGeneratorToken::Type::RandColorSym);
  match(PropertyGeneratorToken::Type::LParen);
  match(PropertyGeneratorToken::Type::RParen);

  std::uniform_real_distribution<float> distribution(0.0, 1.0);

  float r = myWorld.getRandomNumber(distribution);
  float g = myWorld.getRandomNumber(distribution);
  float b = myWorld.getRandomNumber(distribution);

  std::string color = std::to_string(r) + " " + std::to_string(g) + " " +
    std::to_string(b);

  myEvaluationStack.push(color);
}

void QS::PropertyGenerator::statement()
{
  checkInput({PropertyGeneratorToken::Type::RandSym,
              PropertyGeneratorToken::Type::RandColorSym,
              PropertyGeneratorToken::Type::FloatLiteral,
              PropertyGeneratorToken::Type::Id,
              PropertyGeneratorToken::Type::IntLiteral,
              PropertyGeneratorToken::Type::LParen,
              });


  PropertyGeneratorToken peekToken(myScanner->peek());
  switch (peekToken.getToken())
  {
    case PropertyGeneratorToken::Type::RandSym:
      rand();
      break;

    case PropertyGeneratorToken::Type::RandColorSym:
      randColor();
      break;

    case PropertyGeneratorToken::Type::FloatLiteral:
    case PropertyGeneratorToken::Type::Id:
    case PropertyGeneratorToken::Type::IntLiteral:
    case PropertyGeneratorToken::Type::LParen:
    {
      expression();
    }
    break;

    default:
      // Won't get here due to checkInput above, but g++ generates warnings
      // for unhandled enums.
      break;
  }
}

void QS::PropertyGenerator::term()
{
  checkInput({PropertyGeneratorToken::Type::Id,
              PropertyGeneratorToken::Type::IntLiteral,
              PropertyGeneratorToken::Type::LParen,
              PropertyGeneratorToken::Type::FloatLiteral});

  primary();
  termTail();
}

void QS::PropertyGenerator::termTail()
{
  PropertyGeneratorToken peekToken(myScanner->peek());
  if (peekToken.getToken() == PropertyGeneratorToken::Type::MultOp ||
      peekToken.getToken() == PropertyGeneratorToken::Type::DivideOp)
  {
    OperatorRecord operatorRecord(PropertyGeneratorToken::Type::MultOp);
    multOp(operatorRecord);

    primary();

    auto right = myEvaluationStack.top();
    myEvaluationStack.pop();
    auto left = myEvaluationStack.top();
    myEvaluationStack.pop();

    if (operatorRecord.getToken() == PropertyGeneratorToken::Type::MultOp)
    {
      float result = left * right;
      myEvaluationStack.push(result);
    }
    else
    {
      float result = left / right;
      myEvaluationStack.push(result);
    }

    termTail();
  }
}

QS::PropertyGenerator::OperatorRecord::OperatorRecord(Type theOperator) :
  PropertyGeneratorToken(theOperator, 0)
{
  if (theOperator != Type::PlusOp && theOperator != Type::MinusOp &&
      theOperator != Type::MultOp && theOperator != Type::DivideOp)
  {
    throw std::invalid_argument(
      "Illegal token type provided to OperatorRecord, " + getTokenString() +
      ", must be either PlusOp, MinusOp, MultOp, DivideOp.");
  }
}
