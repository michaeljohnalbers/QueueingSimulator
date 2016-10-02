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
    World() = default;

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
     * @param theInterval
     *          amount of time elapsed since last update
     * @return true if the simulation has finished, false otherwise
     */
    bool update(std::chrono::milliseconds theInterval);

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
     * Detects if the Actor has collided with anything in the world.
     *
     * @param theActor
     *          Actor to check for collisions
     * @param theNewPosition
     *          potential new position for the Actor (assuming no collisions)
     */
    void collisionDetection(Actor *theActor,
                            const Eigen::Vector2f theNewPosition) const;

    /** All of the Actors for the simulation. */
    std::vector<Actor*> myActors;

    /** World length (y dimension), in meters.*/
    float myLength_m = 0.0;

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
