#pragma once

/**
 * @file World.h
 * @brief Defines the simulation world.
 *
 * @author Michael Albers
 */

#include <vector>

namespace QS
{
  class Actor;
  class Exit;
  class Target;

  /**
   * The world in the base of a simulation. It contains all of the pieces of
   * the simulation (Actors, etc.). It is also performs the updates that define
   * the simulation.
   */
  class World
  {

    // TODO: still needs more work: constructors, metrics getter, probably more


    public:

    /**
     * Updates the world to the new state.
     *
     * @return true if the simulation has finished, false otherwise
     */
    bool update();

    protected:

    private:

    /**
     * All of the Actors actively being simulated. This is a subset of
     * myActors.
     */
    std::vector<Actor*> myActiveActors;

    /** All of the Actors for the simulation. */
    std::vector<Actor*> myActors;

    /** All exits in the world. */
    std::vector<Exit*> myExits;

    /** World length, in meters.*/
    float myLength_m;

    /** All targets in the world. */
    std::vector<Target*> myTarget;

    /** World width, in meters.*/
    float myWidth_m;

  };
}
