#pragma once

/**
 * @file CollisionAvoidance.h
 * @brief Attempts to prevent collisions between Actors
 *
 * @author Michael Albers
 */

#include "Behavior.h"

namespace QS
{
  /**
   * Attempts to prevent collisions between Actors.
   */
  class CollisionAvoidance : public Behavior
  {
    public:

    /**
     * Default constructor.
     */
    CollisionAvoidance() = delete;

    /**
     * Constructor.
     *
     * @param theProperties
     *          properties for this CollisionAvoidance
     * @param theTag
     *          optional user-defined tag for differentiating
     *          CollisionAvoidances
     */
    CollisionAvoidance(const Properties &theProperties,
                       const std::string &theTag);

    /**
     * Copy constructor.
     */
    CollisionAvoidance(const CollisionAvoidance&) = default;

    /**
     * Move constructor.
     */
    CollisionAvoidance(CollisionAvoidance&&) = default;

    /**
     * Destructor.
     */
    virtual ~CollisionAvoidance() = default;

    /**
     * Attempts to avoid collisions with other Actors.
     *
     * @param theActor
     *          Actor this CollisionAvoidance is working on
     * @return Steering force to apply to the Actor.
     */
    virtual Eigen::Vector2f evaluate(const Actor *theActor) override;

    /**
     * Copy assignment operator.
     */
    CollisionAvoidance& operator=(const CollisionAvoidance&) = default;

    /**
     * Move assignment operator.
     */
    CollisionAvoidance& operator=(CollisionAvoidance&&) = default;

    protected:

    /**
     * Computer a steering force to avoid the given Actor.
     *
     * @param theActor
     *          current Actor this Behavior is working on
     * @param theOther
     *          Actor to avoid
     * @param theOtherPositionAtNearestApproach
     *          position of theOther at nearest approach
     * @return steering force to avoid the other Actor
     */
    Eigen::Vector2f computeAvoidSteeringForce(
      const Actor *theActor,
      const Actor *theOther,
      const Eigen::Vector2f &theOtherPositionAtNearestApproach) const;

    /**
     * Computes how close the two Actors will be in the give number of seconds.
     *
     * @param theActor
     *          current Actor this Behavior is working on
     * @param theOther
     *          other Actor to check against.
     * @param theTime
     *          time from now, seconds (most likely output from
     *          predictNearestApproachTime)
     * @param theOtherPositionAtNearestApproach
     *          OUT parameter, position of theOther at nearest approach
     * @return distance between the two Actor at the given time.
     */
    float computeNearestApproachPositions(
      const Actor *theActor,
      const Actor *theOther,
      float theTime,
      Eigen::Vector2f &theOtherPositionAtNearestApproach) const;

    /**
     * Predicts when the current Actor and the other Actor will be closest.
     *
     * @param theActor
     *          current Actor this Behavior is working on
     * @param theOther
     *          other Actor to check against.
     * @return Amount of time, in seconds, until they will achieve the nearest
     *         approach to each other.
     */
    float predictNearestApproachTime(const Actor *theActor,
                                     const Actor *theOther) const;

    private:
  };
}
