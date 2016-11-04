#pragma once

/**
 * @file ExitDefinition.h
 * @brief Exit definition as defined in the plugin file.
 *
 * @author Michael Albers
 */

#include <string>

namespace QS
{
  /**
   * Contains the definition for one type of Exit as defined in a plugin
   * definition file.
   */
  class ExitDefinition
  {
    public:

    /**
     * Default constructor.
     */
    ExitDefinition() = delete;

    /**
     * Constructor.
     *
     * @param theName
     *          specific name of Exit type
     */
    ExitDefinition(const std::string &theName);

    /**
     * Copy constructor.
     */
    ExitDefinition(const ExitDefinition&) = default;

    /**
     * Move constructor.
     */
    ExitDefinition(ExitDefinition&&) = default;

    /**
     * Destructor.
     */
    ~ExitDefinition() = default;

    /**
     * Returns the name of the type of Exit.
     *
     * @return exit type name
     */
    std::string getName() const noexcept;

    /**
     * Equality operator.
     *
     * @param theOther
     *          exit definition to compare against
     * @return true if the type names are equal, false otherwise
     */
    bool operator==(const ExitDefinition &theOther) const noexcept;

    /**
     * Copy assignment operator.
     */
    ExitDefinition& operator=(const ExitDefinition&) = default;

    /**
     * Move assignment operator.
     */
    ExitDefinition& operator=(ExitDefinition&&) = default;

    protected:

    private:

    /** Name of the type of exit */
    std::string myName;
  };
}
