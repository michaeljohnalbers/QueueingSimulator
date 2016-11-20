#pragma once

/**
 * @file Separation.h
 * @brief Moves an Actor away from Actors in the neighboring area.
 *
 * @author Michael Albers
 */

#include "Behavior.h"

namespace QS
{
  /**
   * Moves an Actor away from Actors in the neighboring area.
   */
  class Separation : public Behavior
  {
    public:

    /**
     * Default constructor.
     */
    Separation() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties for Separation
     * @param theTag
     *          optional user-defined tag for differentiating Separations
     */
    Separation(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    Separation(const Separation&) = default;

    /**
     * Move constructor.
     */
    Separation(Separation&&) = default;

    /**
     * Destructor.
     */
    virtual ~Separation() = default;

    /**
     * Moves the Actor towards an Exit.
     *
     * @param theActor
     *          Actor Separation is working on
     * @return Steering force to apply to the Actor.
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor) override;

    /**
     * Copy assignment operator.
     */
    Separation& operator=(const Separation&) = default;

    /**
     * Move assignment operator.
     */
    Separation& operator=(Separation&&) = default;

    protected:

    private:
  };
}
