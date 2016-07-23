#pragma once

/**
 * @file Sensable.h
 * @brief Contains all things which can be sensed in the simulation.
 *
 * @author Michael Albers
 */

class Actor;
class Target;
class Exit;

namespace QS
{
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
     * @param theTargets
     *          sensable targets within the world
     * @param theExits
     *          sensable exits within the world
     */
    Sensable(const std::vector<const Actor*> theActors,
             const std::vector<const Target*> theTargets,
             const std::vector<const Exit*> theExits);

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

    protected:

    private:

    /** All actors in the world that can be sensed. */
    const std::vector<const Actor*> myActors;

    /** All targets in the world which can be sensed. */
    const std::vector<const Target*> myTargets;

    /** All exits in the world which can be sensed. */
    const std::vector<const Exit*> myExits;
  };
}
