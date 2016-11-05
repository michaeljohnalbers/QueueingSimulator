#pragma once

/**
 * @file PropertyGeneratorToken.h
 * @brief Class which defines a token.
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <map>
#include <ostream>
#include <string>

namespace QS
{
  /**
   * This class defines all known token types as well as representing a single
   * token scanned from source code.
   */
  class PropertyGeneratorToken
  {
    public:

    /** All tokens */
    enum class Type
    {
      RandSym,
      RandColorSym,
      Id,
      IntLiteral,
      FloatLiteral,
      LParen,
      RParen,
      Comma,
      PlusOp,
      MinusOp,
      MultOp,
      DivideOp,
      EofSym,
    };

    /**
     * Default constructor.
     */
    PropertyGeneratorToken() = default;

    /**
     * Constructor (EofSym has no literal)
     *
     * @param theToken
     *          token
     * @param theIndex
     *          index into input on which the token starts
     */
    PropertyGeneratorToken(Type theToken,
                           uint32_t theIndex);

    /**
     * Constructor
     *
     * @param theToken
     *          token
     * @param theTokenLiteral
     *          literal token string from source
     * @param theIndex
     *          index into input on which the token starts
     */
    PropertyGeneratorToken(Type theToken,
                           const std::string &theTokenLiteral,
                           uint32_t theIndex);

    /**
     * Copy constructor.
     */
    PropertyGeneratorToken(const PropertyGeneratorToken&) = default;

    /**
     * Move constructor.
     */
    PropertyGeneratorToken(PropertyGeneratorToken&&) = default;

    /**
     * Destructor
     */
    ~PropertyGeneratorToken() = default;

    /**
     * Returns the index on which the token starts.
     *
     * @return the index on which the token starts.
     */
    uint32_t getIndex() const noexcept;

    /**
     * Returns the token string
     *
     * @return token string
     */
    std::string getLiteral() const noexcept;

    /**
     * Returns the token.
     *
     * @return token
     */
    Type getToken() const noexcept;

    /**
     * Returns a string representation of the token (i.e., "EofSym" for the
     * EofSym token).
     *
     * @return string representation of the token
     */
    std::string getTokenString() const noexcept;

    /**
     * Copy assignment operator
     */
    PropertyGeneratorToken& operator=(const PropertyGeneratorToken &theRHS) =
      default;

    /**
     * Move assignment operator
     */
    PropertyGeneratorToken& operator=(PropertyGeneratorToken &&theRHS) =
      default;

    /**
     * Less than operator. There is no natural ordering to tokens, this is just
     * provided so this class can be used in ordered STL containers.
     *
     * @param theRHS
     *          right hand side of <
     * @return true if less than, false otherwise
     */
    bool operator<(const PropertyGeneratorToken &theRHS) const noexcept;

    /**
     * Equality operator.
     *
     * @param theRHS
     *          right hand side of ==
     * @return true if equal, false otherwise
     */
    bool operator==(const PropertyGeneratorToken &theRHS) const noexcept;

    /**
     * Insertion operator
     *
     * @param theOS
     *          stream in which to insert data
     * @param theToken
     *          token data
     * @return modified stream
     */
    friend std::ostream& operator<<(std::ostream &theOS,
                                    const PropertyGeneratorToken &theToken);

    protected:

    private:

    /** Index (character in input) on which the token starts. */
    uint32_t myIndex = 0;

    /** Token type */
    Type myToken = Type::EofSym;

    /** Token literal from source. */
    std::string myTokenLiteral;

    using TokenMap = std::map<Type, std::string>;

    /** Token-to-string mapping*/
    static TokenMap ourTokenDescriptions;

  };
}
