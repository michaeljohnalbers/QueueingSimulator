#pragma once

/**
 * @file PropertyGeneratorScanner.h
 * @brief Defines the class used to scan input for tokens
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <map>
#include <string>

#include "PropertyGeneratorToken.h"

namespace QS
{
  /**
   * Scans an input string returning all of the tokens contained therein. It
   * supports a one token look ahead feature (i.e., peeking).  Use nextToken to
   * find the next token in the file. Use peek to return the look ahead token.
   */
  class PropertyGeneratorScanner
  {
    public:

    /**
     * Default constructor.
     */
    PropertyGeneratorScanner() = delete;

    /**
     * Constructor
     *
     * @param theInput
     *          string to scan/tokenize
     */
    PropertyGeneratorScanner(const std::string &theInput);

    /**
     * Copy constructor
     */
    PropertyGeneratorScanner(const PropertyGeneratorScanner &) = default;

    /**
     * Move constructor
     */
    PropertyGeneratorScanner(PropertyGeneratorScanner &&) = default;

    /**
     * Destructor
     */
    ~PropertyGeneratorScanner() = default;

    /**
     * Returns the last scanned token. Will be EofSym until nextToken has been
     * called.
     *
     * @return token from file
     */
    PropertyGeneratorToken getCurrentToken() const;

    /**
     * Find and return the next token in the file. Updates current token.
     *
     * @return next token
     * @throw std::runtime_error
     *          on syntax error
     */
    PropertyGeneratorToken nextToken();

    /**
     * Copy assignment operator
     */
    PropertyGeneratorScanner& operator=(const PropertyGeneratorScanner &) =
      default;

    /**
     * Move assignment operator
     */
    PropertyGeneratorScanner& operator=(PropertyGeneratorScanner &&) = default;

    /**
     * Returns the next token, but does not advance the token stream.
     *
     * @return next token
     * @throw std::runtime_error
     *          on syntax error
     */
    PropertyGeneratorToken peek();

    protected:

    private:

    /** Maximum number of characters in an identifier. */
    static constexpr uint32_t MAX_ID_LENGTH = 32;

    /**
     * Adds the given character to the token buffer.
     *
     * @param theCharacter
     *          character to add
     */
    void addCharacterToBuffer(char theCharacter);

    /**
     * Reads and discards current character in file.
     */
    void advance();

    /**
     * Clears the token buffer.
     */
    void clearBuffer();

    /**
     * Returns the token type of an identifier.
     *
     * @param theTokenLiteral
     *          token literal from source
     * @param theTokenStartIndex
     *          starting index of the token
     * @return Id or specific reserved word token
     */
    PropertyGeneratorToken idOrReserved(const std::string &theTokenLiteral,
                                        uint32_t theTokenStartIndex);

    /**
     * Returns current character in the input file. Doesn't advance. Essentially
     * a 'peek' operation.
     *
     * @return current character
     */
    char inspectCharacter();

    /**
     * For when a newline is encountered.
     */
    void newLine();

    /**
     * read next character in the file and advance
     *
     * @return next charcter
     */
    char readCharacter();

    /**
     * Reads and extracts the next token from the input file
     *
     * @return next token
     * @throw std::runtime_error
     *          on syntax error
     */
    PropertyGeneratorToken readToken();

    /**
     * Throws an exception for a syntax error
     *
     * @param theError
     *          error description
     * @throws always throws a std::runtime_error
     */
    void syntaxError(const std::string &theError);

    using ReservedWordMap = std::map<std::string, PropertyGeneratorToken::Type>;

    /** Reserved words */
    static ReservedWordMap ourReservedWords;

    /** Token literal being assembled */
    std::string myBuffer;

    /** Last scanned token. */
    PropertyGeneratorToken myCurrentToken;

    /** Index into myInput */
    uint32_t myIndex;

    /** Input string */
    std::string myInput;

    /** Look ahead token. */
    PropertyGeneratorToken myPeekToken;

    /**
     * Pointer to look ahead token. Used to determine if peek token has
     * been initialized.
     */
    PropertyGeneratorToken *myPeekTokenPtr = nullptr;
  };
}
