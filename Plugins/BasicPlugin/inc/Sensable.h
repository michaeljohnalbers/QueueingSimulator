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
  // TODO: undo this with further development
#if 0
  class Target;
  class Exit;
#endif

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
     * @param theActors
     *          sensable actors within the world
     * @param theIntervalInSeconds
     *          time since last update
     * @param theTargets
     *          sensable targets within the world
     * @param theExits
     *          sensable exits within the world
     */
    Sensable(const std::vector<const Actor*> theActors,
             float theIntervalInSeconds
             // TODO: undo this with further developement
#if 0
             ,
             const std::vector<const Target*> theTargets,
             const std::vector<const Exit*> theExits
#endif
      ) noexcept;

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
    std::vector<const Actor*> getActors() const noexcept;

    /**
     * Returns the interval, in seconds, since the last update.
     *
     * @return the interval, in seconds, since the last update.
     */
    float getIntervalInSeconds() const noexcept;

    // TODO: undo this with further development
#if 0
    /**
     * Returns the sensable targets. Not in any particular order.
     *
     * @return sensable targets
     */
    std::vector<const Target*> getTargets() const noexcept;

    /**
     * Returns the sensable exits. Not in any particular order.
     *
     * @return sensable exits.
     */
    std::vector<const Exit*> getExits() const noexcept;

#endif

    /**
     * Copy assignment operator
     */
    Sensable& operator=(const Sensable &) = default;

    /**
     * Move assignment operator
     */
    Sensable& operator=(Sensable &&) = default;


    protected:

    private:

    /** All actors in the world that can be sensed. */
    std::vector<const Actor*> myActors;

    /** Interval since last update. */
    float myIntervalInSeconds;

    // TODO: undo this with further development
#if 0
    /** All targets in the world which can be sensed. */
    std::vector<const Target*> myTargets;

    /** All exits in the world which can be sensed. */
    std::vector<const Exit*> myExits;
#endif
  };
}
