#pragma once

/**
 * @file Sensable.h
 * @brief Contains all things which can be sensed in the simulation.
 *
 * @author Michael Albers
 */

#include <vector>

namespace QS
{
  class Actor;
  class Exit;

  /**
   * This class encapsulates all things that can be sensed within within the
   * simulation world. In other words, all Sensors use objects of this class
   * as input when "sensing" the simulation world.
   */
  class Sensable
  {
    public:

    /**
     * Default constructor.
     */
    Sensable() = delete;

    /**
     * Constructor.
     *
     * @param theCurrentActor
     *          current actor "sensing" the world
     * @param theActors
     *          sensable actors within the world
     * @param theExits
     *          sensable exits within the world
     * @param theIntervalInSeconds
     *          time since last update
     */
    Sensable(const Actor *theCurrentActor,
             const std::vector<const Actor*> &theActors,
             const std::vector<const Exit*> &theExits,
             float theIntervalInSeconds) noexcept;

    /**
     * Copy constructor
     */
    Sensable(const Sensable &) = default;

    /**
     * Move constructor
     */
    Sensable(Sensable &&) = default;

    /**
     * Destructor.
     */
    ~Sensable() = default;

    /**
     * Returns the sensable actors. Not in any particular order.
     *
     * @return sensable actors.
     */
    const std::vector<const Actor*>& getActors() const noexcept;

    /**
     * Returns the Actor "sensing" the world.
     *
     * @return actor
     */
    const Actor* getCurrentActor() const noexcept;

    /**
     * Returns the sensable exits. Not in any particular order.
     *
     * @return sensable exits.
     */
    const std::vector<const Exit*>& getExits() const noexcept;

    /**
     * Returns the interval, in seconds, since the last update.
     *
     * @return the interval, in seconds, since the last update.
     */
    float getIntervalInSeconds() const noexcept;

    /**
     * Copy assignment operator
     */
    Sensable& operator=(const Sensable &) = delete;

    /**
     * Move assignment operator
     */
    Sensable& operator=(Sensable &&) = delete;

    protected:

    private:

    /** All actors in the world that can be sensed. */
    const std::vector<const Actor*> &myActors;

    /** Current Actor "sensing" the world. */
    const Actor *myCurrentActor;

    /** All exits in the world which can be sensed. */
    const std::vector<const Exit*> &myExits;

    /** Interval since last update. */
    float myIntervalInSeconds;
  };
}
