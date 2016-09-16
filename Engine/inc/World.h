#pragma once

/**
 * @file World.h
 * @brief Defines the simulation world.
 *
 * @author Michael Albers
 */

#include <vector>
#include <random>

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
     * Sets the dimensions of the world.
     *
     * @param theWidth_m
     *          width of the world, in meters
     * @param theLength_m
     *          length of the world, in meters
     */
    void setDimentions(float theWidth_m, float theLength_m);

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
     * @return true if the simulation has finished, false otherwise
     */
    bool update();

    protected:

    private:

    /** All of the Actors for the simulation. */
    std::vector<Actor*> myActors;

    /** World length, in meters.*/
    float myLength_m = 0.0;

    /** World width, in meters.*/
    float myWidth_m = 0.0;

    /** Generator of pseudo-random numbers. */
    std::mt19937 myRNGEngine;

    /** Distribution of pseudo-random numbers. */
    std::uniform_real_distribution<> myRNGDistribution{0, 1};
  };
}
