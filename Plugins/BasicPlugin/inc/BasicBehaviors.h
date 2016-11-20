#pragma once

/**
 * @file BasicBehaviors.h
 * @brief Basic behaviors as described in Steering Behaviors for Autonomous
 * Characters. The Behaviors in this class use no sensors or any other input
 * besides the given parameters.
 *
 * @author Michael Albers
 */

#include "Eigen/Core"

namespace QS
{
  class Actor;

  class BasicBehaviors
  {
    public:

    /**
     * Generates a force to move the Actor towards the given position, slowing
     * down as the Actor approaches.
     *
     * @param theActor
     *          Actor being moved
     * @param thePosition
     *          targeted position
     * @param theSlowingRadius
     *          radius from thePosition at which point the Actor starts slowing
     * @return force to move the Actor to the position
     * @throws std::invalid_argument
     *          if theSlowingRadius is negative
     */
    static Eigen::Vector2f arrival(const Actor *theActor,
                                   const Eigen::Vector2f &thePosition,
                                   float theSlowingRadius);

    /**
     * Generates a force to move the Actor away from another Actor.
     *
     * @param theActor
     *          Actor being moved
     * @param theActorToEvade
     *          Actor theActor is evading
     * @return force to move the Actor away from the other Actor
     */
    static Eigen::Vector2f evade(const Actor *theActor,
                                 const Actor *theActorToEvade) noexcept;

    /**
     * Generates a force to move the Actor away from the given position.
     *
     * @param theActor
     *          Actor being moved
     * @param thePosition
     *          targeted position
     * @param theDesiredSpeed
     *          speed at which to move away from object
     * @return force to move the Actor awary fom the position
     */
    static Eigen::Vector2f flee(const Actor *theActor,
                                const Eigen::Vector2f &thePosition,
                                float theDesiredSpeed) noexcept;

    /**
     * Generates a force to move the Actor toward another Actor.
     *
     * @param theActor
     *          Actor being moved
     * @param theActorToEvade
     *          Actor theActor is pursuing
     * @return force to move the Actor toward the other Actor
     */
    static Eigen::Vector2f pursuit(const Actor *theActor,
                                   const Actor *theActorToEvade) noexcept;

    /**
     * Generates a force to move the Actor towards the given position.
     *
     * @param theActor
     *          Actor being moved
     * @param thePosition
     *          targeted position
     * @param theDesiredSpeed
     *          speed at which to move away from object
     * @return force to move the Actor to the position
     */
    static Eigen::Vector2f seek(const Actor *theActor,
                                const Eigen::Vector2f &thePosition,
                                float theDesiredSpeed) noexcept;

    /**
     * Generates a steering force to separate the given Actor from the other
     * Actors.
     *
     * @param theActor
     *          Actor trying to separate from the others
     * @param theActors
     *          Actors to separate from
     * @param theSeparationDistance
     *          Scalaing factor to increase or decrease the separation distance
     * @return force to separate the Actor from the others.
     * @throws std::invalid_argument
     *         if theSeparationDistance is negative
     */
    static Eigen::Vector2f separation(
      const Actor *theActor,
      const std::vector<const Actor*> &theActors,
      float theSeparationDistance);

    protected:

    /**
     * Does all the work for evade &amp; pursuit.
     *
     * @param theActor
     *          Actor being moved
     * @param theOtherActor
     *          Actor theActor is evading or pursuing
     * @param theEvade
     *          true == evade, false == pursuit
     * @return force to move the Actor toward or away from the other Actor
     */
    static Eigen::Vector2f evadePursuitHelper(const Actor *theActor,
                                              const Actor *theOtherActor,
                                              bool theEvade) noexcept;
   

    /**
     * Guts of the seek &amp; flee behaviors since they are identical excepting
     * the desired velocity.
     *
     * @param theActor
     *          Actor being moved
     * @param theDesiredVelocity
     *          direction to move
     * @param theDesiredSpeed
     *          speed at which to move to/away from the object
     * @return force to move the Actor towards the desired velocity
     */
    static Eigen::Vector2f seekFleeHelper(const Actor *theActor,
                                          Eigen::Vector2f theDesiredVelocity,
                                          float theDesiredSpeed)
      noexcept;
  };
}
