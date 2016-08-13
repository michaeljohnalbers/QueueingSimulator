#pragma once

/**
 * @file Exit.h
 * @brief Specialized Exit that adds the semantics of removing an actor from
 *        the simulation.
 *
 * @author Michael Albers
 */

#include "Target.h"

namespace QS
{
  /**
   * An exit is just a target with the added semantics that when an Actor
   * overlaps an exit it can be removed from the simulation.
   */
  class Exit: public Target
  {
    public:

    /**
     * Default constructor.
     */
    Exit() = delete;

    /**
     * Constructor. See Target.h
     *
     */
    Exit(const Eigen::Vector2f &thePoint, const float theRadius);

    /**
     * Copy constructor.
     */
    Exit(const Exit&) = default;

    /**
     * Move constructor.
     */
    Exit(Exit &&) = default;

    /**
     * Destructor
     */
    ~Exit() = default;

    /**
     * Copy assignment operator.
     */
    Exit& operator=(const Exit&) = default;

    /**
     * Move assignment operator.
     */
    Exit& operator=(Exit &&) = default;
  };
}
