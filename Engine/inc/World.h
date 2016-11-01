#pragma once

/**
 * @file World.h
 * @brief Defines the simulation world.
 *
 * @author Michael Albers
 */

#include <chrono>
#include <random>
#include <tuple>
#include <vector>
#include "Eigen/Core"

namespace QS
{
  class Actor;
  class Metrics;

  /**
   * The world in the base of a simulation. It contains all of the pieces of
   * the simulation (Actors, etc.). It is also performs the updates that define
   * the simulation.
   */
  class World
  {
    public:

    /**
     * Default constructor.
     */
    World() = delete;

    /**
     * Constructor.
     *
     * @param theMetrics
     *          metrics for the simulation, to be updated by this object
     */
    World(Metrics &theMetrics);

    /**
     * Copy constructor.
     */
    World(const World&) = delete;

    /**
     * Move constructor.
     */
    World(World&&) = delete;

    /**
     * Destructor
     */
    ~World() = default;

    /**
     * Add the given Actor to the world.
     *
     * @param theActor
     *          Actor to add
     */
    void addActor(Actor *theActor);

    /**
     * Converts the given point in Actor local space to world space coordinates.
     *
     * @param theActor
     *          Actor to use as local space
     * @param thePoint
     *          point in local space to convert to world space
     */
    static Eigen::Vector2f convertPointToWorld(const Actor *theActor,
                                               const Eigen::Vector2f &thePoint)
      noexcept;

    /**
     * Finalizes metrics for all Actors.
     */
    void finalizeActorMetrics() const noexcept;

    /**
     * Returns all the Actors in the world.
     *
     * @return all the Actors in the world.
     */
    const std::vector<Actor*>& getActors() const noexcept;

    /**
     * Returns the dimensions of the world.
     *
     * @return world dimensions (witdth in meters, legnth in meters)
     */
    std::tuple<float, float> getDimensions() const noexcept;

    /**
     * Returns a random number using the given distribution. Be sure to have
     * called setSeed before using this function. If it hasn't been called
     * the RNG will use the RNG implementation default seed.
     *
     * @param theDistribution
     *          distribution to use
     * @return random number
     */
    float getRandomNumber(
      std::uniform_real_distribution<float> &theDistribution) noexcept;

    /**
     * Initializes the Actor metrics in the Metrics object. This function should
     * only be called once all Actors have been added to the world.
     */
    void initializeActorMetrics() noexcept;

    /**
     * Sets the dimensions of the world.
     *
     * @param theWidth_m
     *          width of the world, in meters
     * @param theLength_m
     *          length of the world, in meters
     */
    void setDimensions(float theWidth_m, float theLength_m);

    /**
     * Seeds the random number generator with the given value.
     *
     * @param theSeed
     *          new random number generator seed.
     */
    void setSeed(uint64_t theSeed);

    /**
     * Copy assignment operator
     */
    World& operator=(const World&) = delete;

    /**
     * Move assignment operator
     */
    World& operator=(World&&) = delete;

    /**
     * Updates the world to the new state.
     *
     * @param theIntervalInSeconds
     *          amount of time elapsed since last update
     * @return true if the simulation has finished, false otherwise
     */
    bool update(float theIntervalInSeconds);

    protected:

    private:

    /**
     * Checks if the initial placement of the provided Actor is valid: in world
     * bounds, Actor hasn't already been added and  not overlapping any other
     * Actor.
     *
     * @param theActor
     *          actor whose placement is to be checked
     * @param std::logic_error
     *          if the Actor's location is invalid
     */
    void checkInitialPlacement(const Actor *theActor) const;

    /**
     * Detects if the Actor has collided with anything in the world. And, if so
     * modifies the position vector to the closest non-collision position. This
     * function will not change the direction of the Actor, just the
     * magnitude, possibly reducing it to zero (i.e., not moving).
     *
     * @param theActor
     *          Actor to check for collisions
     * @param theNewPosition
     *          new position of the Actor in the world
     * @param theCollisionDetected
     *          OUT parameter, true if a collision was detected, false otherwise
     * @return possibly modified position vector based on any collisions
     */
    Eigen::Vector2f collisionDetection(Actor *theActor,
                                       Eigen::Vector2f theNewPosition,
                                       bool &theCollisionDetected)
      const;

    /** All of the Actors for the simulation. */
    std::vector<Actor*> myActors;

    /** World length (y dimension), in meters.*/
    float myLength_m = 0.0;

    /** Metrics for the simulation. */
    Metrics &myMetrics;

    /** World width (x dimension), in meters.*/
    float myWidth_m = 0.0;

    /** Generator of pseudo-random numbers. */
    std::mt19937 myRNGEngine;

    /** Distribution of pseudo-random numbers. */
    std::uniform_real_distribution<> myRNGDistribution{0, 1};

    /** Actors for use in Sensable objects. */
    std::vector<const Actor*> mySensableActors;
  };
}
