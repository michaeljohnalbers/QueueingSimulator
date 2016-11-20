#pragma once

/**
 * @file NearExitArrival.h
 * @brief Moves an Actor towards an Exit.
 *
 * @author Michael Albers
 */

#include "Behavior.h"

namespace QS
{
  /**
   * Moves an Actor towards a spot near the exit along its current path, slowing
   * as the Actor nears this point.
   */
  class NearExitArrival : public Behavior
  {
    public:

    /**
     * Default constructor.
     */
    NearExitArrival() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties for NearExitArrival
     *
     *          Optional properties:
     *           "distance", desired distance from the exit to arrive at, in
     *             meters. If less than zero, or not given, 3 * the Exit's
     *             radius is used.
     * @param theTag
     *          optional user-defined tag for differentiating NearExitArrivals
     */
    NearExitArrival(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    NearExitArrival(const NearExitArrival&) = default;

    /**
     * Move constructor.
     */
    NearExitArrival(NearExitArrival&&) = default;

    /**
     * Destructor.
     */
    virtual ~NearExitArrival() = default;

    /**
     * Moves the Actor towards an Exit.
     *
     * @param theActor
     *          Actor NearExitArrival is working on
     * @return Steering force to apply to the Actor.
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor) override;

    /**
     * Returns true if at any point the Actor has reached its position (even
     * if the Actor later moved away from it).
     *
     * @return true if the position has been reached
     */
    bool getAtPosition() const noexcept;

    /**
     * Copy assignment operator.
     */
    NearExitArrival& operator=(const NearExitArrival&) = default;

    /**
     * Move assignment operator.
     */
    NearExitArrival& operator=(NearExitArrival&&) = default;

    protected:

    private:

    /**
     * Distance from exit to arrive at. If not defined uses a value based on
     * the exit.
     */
    float myDesiredDistanceFromExit;

    /** Has the Actor reached the position? */
    bool myAtPosition = false;
  };
}
