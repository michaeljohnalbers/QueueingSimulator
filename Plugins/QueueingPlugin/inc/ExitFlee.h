#pragma once

/**
 * @file ExitFlee.h
 * @brief Moves an Actor away from an Exit.
 *
 * @author Michael Albers
 */

#include "Behavior.h"

namespace QS
{
  /**
   * Moves an Actor away from an Exit.
   */
  class ExitFlee : public Behavior
  {
    public:

    /**
     * Default constructor.
     */
    ExitFlee() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties for ExitFlee
     * @param theTag
     *          optional user-defined tag for differentiating ExitFlees
     */
    ExitFlee(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    ExitFlee(const ExitFlee&) = default;

    /**
     * Move constructor.
     */
    ExitFlee(ExitFlee&&) = default;

    /**
     * Destructor.
     */
    virtual ~ExitFlee() = default;

    /**
     * Moves the Actor away from an Exit.
     *
     * @param theActor
     *          Actor ExitFlee is working on
     * @return Steering force to apply to the Actor.
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor) override;

    /**
     * Copy assignment operator.
     */
    ExitFlee& operator=(const ExitFlee&) = default;

    /**
     * Move assignment operator.
     */
    ExitFlee& operator=(ExitFlee&&) = default;

    protected:

    private:
    
  };
}
