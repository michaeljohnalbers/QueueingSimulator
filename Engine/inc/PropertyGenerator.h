#pragma once

/**
 * @file PropertyGenerator.h
 * @brief Class for generating property values form a configuration file.
 *
 * @author Michael Albers
 */

#include <memory>
#include <set>
#include <stack>
#include <string>
#include "PropertyGeneratorScanner.h"
#include "PropertyGeneratorToken.h"

namespace QS
{
  class SimulationEntityConfiguration;
  class World;

  class PropertyGenerator
  {
    public:

    /**
     * Default constructor
     */
    PropertyGenerator() = delete;

    /**
     * Constructor.
     *
     * @param theWorld
     *          used for random number generation and for attribute reference
     */
    PropertyGenerator(World &theWorld);

    /**
     * Copy constructor.
     */
    PropertyGenerator(const PropertyGenerator&) = default;

    /**
     * Move constructor.
     */
    PropertyGenerator(PropertyGenerator&&) = default;

    /**
     * Destructor
     */
    ~PropertyGenerator() = default;

    /**
     * Generates the property value based off the provided value (likely read
     * from the simulation configuration file). If the first character is an
     * ':' then the input is treated as generator syntax and
     * parsed/executed. Otherwise the value is returned unmodified.
     *
     * The syntax of the properties is a C-like syntax for mathematical
     * expressions. +,-,* and / are supported with standard precedence and
     * associativity. Parenthesized expressions are also supported. Numbers can
     * be integers or floats (only supported syntax is abc.xyz). All values
     * however are treated as floats when evaluating the expression, and are
     * returned as such.
     *
     * A limited number of built in variables are available. They are:
     *   world.width/world.length - Used to get the world width and length,
     *     respectively.
     *   this.* - References any already defined property for the current
     *     simulation entity being processed
     *   PI - PI
     *
     * Lastly, random numbers can be generated. To do so, use a simulated
     * function call: "rand(&lt;low&gt;, &lt;high&gt;). This returns a floating
     * point number in the given range. This cannot be part of an expression:
     * it must be used standalone. This may be improved in the future.
     *
     * @param theValue
     *          initial property value
     * @param theCurrentEntity
     *          current simulation entity being processed, used for property
     *          retrieval
     * @return property value
     */
    std::string generateProperty(
      const std::string &theValue,
      const SimulationEntityConfiguration &theCurrentEntity);

    /**
     * Copy assignment operator
     */
    PropertyGenerator& operator=(const PropertyGenerator&) = default;

    /**
     * Move assignment operator
     */
    PropertyGenerator& operator=(PropertyGenerator&&) = default;

    protected:

    private:

    /**
     * Throws a runtime_exception with an error message built from the given
     * parameters.
     *
     * @param theToken
     *          current token, used for token data and string index
     * @param theError
     *          detailed error information
     * @throws std::runtime_exception
     *           always thrown
     */
    void error(const PropertyGeneratorToken &theToken,
               const std::string &theError);

    /** This class tracks which operator is used. */
    class OperatorRecord : public PropertyGeneratorToken
    {
      public:

      OperatorRecord() = delete;
      OperatorRecord(const OperatorRecord &) = default;
      OperatorRecord(OperatorRecord &&) = default;

      /**
       * Constructor
       *
       * @param theOperator
       *          token (must be either plus or minus)
       * @throws std::invalid_argument on invalid token type
       */
      OperatorRecord(Type theOperator);

      virtual ~OperatorRecord() = default;
      OperatorRecord& operator=(const OperatorRecord &theRHS) = default;
      OperatorRecord& operator=(OperatorRecord &&theRHS) = default;

      protected:
      private:
    };

    /**
     * Parse/execute the value.
     *
     * @throws std::runtime_error
     *           syntax error
     */
    void parse();

    /*
     * Production functions
     */
    void addOp(OperatorRecord &theOperator);
    void expression();
    void expressionTail();
    void ident();
    void multOp(OperatorRecord &theOperator);
    void rand();
    void primary();
    void statement();
    void term();
    void termTail();

    /*
     * Other functions
     */
    using TokenSet = std::set<PropertyGeneratorToken::Type>;

    /**
     * Checks if the next token in the input is one of the provided tokens.
     *
     * @param theValidSet
     *          set of valid tokens
     * @throws std::runtime_error
     *          syntax error
     */
    void checkInput(const TokenSet &theValidSet);

    /**
     * Extracts the next token and matches it against the provided token. Syntax
     * error is reported if there is no match.
     *
     * @param theToken
     *          token to match against
     * @throws std::runtime_error
     *          syntax error
     */
    void match(const PropertyGeneratorToken::Type &theToken);

    /** Current entity from which to get attributes. */
    const SimulationEntityConfiguration *myEntity;

    /** Evaluation stack. */
    std::stack<float> myEvaluationStack;

    /** Original property passed in for processing. */
    std::string myOriginalProperty;

    /** Scanner */
    std::unique_ptr<PropertyGeneratorScanner> myScanner;

    /** Used for generating random numbers and referencing attributes. */
    World &myWorld;
  };
}
