#pragma once

/**
 * @file World.h
 * @brief Defines the simulation world.
 *
 * @author Michael Albers
 */

#include <chrono>
#include <cstdint>
#include <random>
#include <tuple>
#include <vector>
#include "Eigen/Core"

namespace QS
{
  class Actor;
  class Exit;
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
     * @throw std::logic_error
     *          if the Actor has already been added or it's position is in any
     *          way invalid
     */
    void addActor(Actor *theActor);

    /**
     * Add the given Exit to the world.
     *
     * @param theExit
     *          Exit to add
     * @throw std::logic_error
     *          if the Exit has already been added or it's position is in any
     *          way invalid
     */
    void addExit(Exit *theExit);

     /**
     * Checks if the circle defined by the given position/radius pairs overlap
     * with each other.
     *
     * @param thePosition1
     *          first circle center point
     * @param theRadius1
     *          first cirlce radius
     * @param thePositionw
     *          secondcircle center point
     * @param theRadius1
     *          second cirlce radius
     * @return true if the circles overlap
     */
    bool checkOverlap(Eigen::Vector2f thePosition1, float theRadius1,
                      Eigen::Vector2f thePosition2, float theRadius2)
      const noexcept;

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
     * Finalizes all metrics.
     */
    void finalizeMetrics() const noexcept;

    /**
     * Returns all the Actors in the simulation.
     *
     * @return all the Actors in the simulation.
     */
    const std::vector<Actor*>& getActors() const noexcept;

    /**
     * Returns all the Actors still in the world. This is a subset of the
     * return of getActors. Actors can be removed from the world if they
     * exit it.
     *
     * @return all the Actors in the world
     */
    const std::vector<const Actor*>& getActorsInWorld() const noexcept;

    /**
     * Returns the dimensions of the world.
     *
     * @return world dimensions (witdth in meters, legnth in meters)
     */
    std::tuple<float, float> getDimensions() const noexcept;

    /**
     * Returns all the Exits in the world.
     *
     * @return all the Exits in the world.
     */
    const std::vector<Exit*>& getExits() const noexcept;

    /**
     * Returns a random, valid position for an Actor with the given radius. The
     * algorithm used by this function could run infinitely if an unlucky set of
     * random numbers are generated, or if the given radius will always overlap
     * an existing Actor. To that end, the caller can bound the number of
     * attempts.
     *
     * @param theRadius
     *          actor radius
     * @param theMaxAttempts
     *          maximum number of tries to find a valid position
     * @return position
     * @throws std::invalid_argument
     *           if theRadius is larger than the world's width or length
     * @throws std::logic_error
     *           if the number of attempts would exceed the maximum
     */
    Eigen::Vector2f getRandomActorPosition(float theRadius,
                                           uint32_t theMaxAttempts);

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

    /**
     * Checks if the given entity is wholly within the world.
     *
     * @param theEntity
     *          entity to check
     * @return true if the entity is completely within the world, false
     *         otherwise
     */
    template<class T>
    bool isInWorld(const T &theEntity) const noexcept;

    /** All of the Actors for the simulation. */
    std::vector<Actor*> myActors;

    /** Actors which are still in the world. */
    std::vector<const Actor*> myActorsInWorld;

    /** All of the Exits in the simulation. */
    std::vector<Exit*> myExits;

    /** Exits specifically for Sensable*/
    std::vector<const Exit*> myExitsForSensable;

    /** World length (y dimension), in meters.*/
    float myLength_m = 0.0;

    /** Metrics for the simulation. */
    Metrics &myMetrics;

    /** World width (x dimension), in meters.*/
    float myWidth_m = 0.0;

    /** Number Actors attempted to be added. */
    uint32_t myNumberAttemptedActorAdds = 0;

    /** Generator of pseudo-random numbers. */
    std::mt19937 myRNGEngine;

    /** Distribution of pseudo-random numbers. */
    std::uniform_real_distribution<> myRNGDistribution{0, 1};
  };
}
