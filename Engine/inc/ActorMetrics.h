#pragma once

/**
 * @file ActorMetrics.h
 * @brief Contains metrics for a single Actor in a simulation.
 *
 * @author Michael Albers
 */

#include "Eigen/Core"

namespace QS
{
  class Actor;

  /**
   * This class keeps metrics for a single Actor in the simulation.
   */
  class ActorMetrics
  {
    public:

    /**
     * Output operator.
     */
    friend std::ostream& operator<<(std::ostream &os,
                                    const ActorMetrics &theMetrics);

    /**
     * Default constructor.
     */
    ActorMetrics() = default;

    /**
     * Constructor.
     *
     * @param theActor
     *          Actor this class is gathering metrics on. The Actor's current
     *          position is saved as the starting ponit for net distance
     *          calculation. So if the Actor has already move the distances
     *          traveled will not be correct.
     *
     *          This pointer is NOT retained in any way by this class.
     */
    ActorMetrics(const Actor *theActor);

    /**
     * Copy constructor.
     */
    ActorMetrics(const ActorMetrics&) = default;

    /**
     * Move constructor.
     */
    ActorMetrics(ActorMetrics&&) = default;

    /**
     * Destructor.
     */
    ~ActorMetrics() = default;

    /**
     * Adds the provided number of meters to the gross distance traveled by this
     * Actor.
     *
     * @param theDistanceMeters
     *          distance to add
     * @throws std::invalid_argument
     *           if theDistanceMeters is negative
     */
    void addGrossDistance(float theDistanceMeters);

    /**
     * Takes the current position of the provided Actor and subtracts it from
     * the saved starting position for net distance calculation.
     *
     * @param theActor
     *          same actor as provided in the constructor.
     * @return net distance
     */
    float calculateNetDistance(const Actor *theActor);

    /**
     * Returns the gross distance traveled, in meters.
     *
     * @return the gross distance traveled, in meters.
     */
    float getGrossDistanceMeters() const noexcept;

    /**
     * Copy assignment operator
     */
    ActorMetrics& operator=(const ActorMetrics&) = default;

    /**
     * Move assignment operator.
     */
    ActorMetrics& operator=(ActorMetrics&&) = default;

    protected:

    /** Gross distance traveled by the Actor. */
    float myGrossDistanceMeters = 0.0;

    /** Net distance travled by the Actor. */
    float myNetDistanceMeters = 0.0;

    /** Starting position of the Actor. */
    Eigen::Vector2f myStartPosition;

    private:
  };
}
