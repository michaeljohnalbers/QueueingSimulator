#pragma once

/**
 * @file ExitSeek.h
 * @brief Moves an Actor towards an Exit.
 *
 * @author Michael Albers
 */

#include "Behavior.h"

namespace QS
{
  /**
   * Moves an Actor towards an Exit.
   */
  class ExitSeek : public Behavior
  {
    public:

    /**
     * Default constructor.
     */
    ExitSeek() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties for ExitSeek
     * @param theTag
     *          optional user-defined tag for differentiating ExitSeeks
     */
    ExitSeek(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    ExitSeek(const ExitSeek&) = default;

    /**
     * Move constructor.
     */
    ExitSeek(ExitSeek&&) = default;

    /**
     * Destructor.
     */
    virtual ~ExitSeek() = default;

    /**
     * Moves the Actor towards an Exit.
     *
     * @param theActor
     *          Actor ExitSeek is working on
     * @return Steering force to apply to the Actor.
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor) override;

    /**
     * Copy assignment operator.
     */
    ExitSeek& operator=(const ExitSeek&) = default;

    /**
     * Move assignment operator.
     */
    ExitSeek& operator=(ExitSeek&&) = default;

    protected:

    private:
  };
}
