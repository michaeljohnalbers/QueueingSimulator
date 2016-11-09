#pragma once

/**
 * @file Exit.h
 * @brief Specialized Target that adds the semantics of removing an actor from
 *        the simulation.
 *
 * @author Michael Albers
 */

#include "Target.h"

namespace QS
{
  class Actor;

  /**
   * An exit is just a target with the added semantics that when an Actor
   * overlaps an exit it can (potentially) be removed from the simulation.
   */
  class Exit: public Target
  {
    public:

    /**
     * Default constructor.
     */
    Exit() = delete;

    /**
     * Constructor. See Target.h for required properties.
     *
     *          Optional properties:
     *            "color", space separated RGB values, each in the range 0.0-1.0
     *               (Example: "0.5 1.0 0.2"). If not given, defaults to (1,1,1)
     *
     * @throws std::invalid_argument
     *           in addition to Target's throw, an exception is thrown in
     *           theRadius is zero.
     */
    Exit(const Properties &theProperties, const std::string &theTag);

    /**
     * Copy constructor.
     */
    Exit(const Exit&) = default;

    /**
     * Move constructor.
     */
    Exit(Exit &&) = default;

    /**
     * Destructor
     */
    virtual ~Exit() = default;

    /**
     * Checks if the Actor can exit through this one. The base implementation
     * always returns true if the Actor's center point is in the exit's radius.
     *
     * @param theActor
     *          Actor to check
     * @return true if the Actor can exit, false otherwise
     */
    virtual bool canActorExit(const Actor *theActor) noexcept;

    /**
     * Returns the color of the Exit. The vector elements are the R, G, B
     * values, respectively. Each is a range suitable for OpenGL (0.0-1.0).
     *
     * @return the color of the Exit
     */
    Eigen::Vector3f getColor() const noexcept;

    /**
     * Copy assignment operator.
     */
    Exit& operator=(const Exit&) = default;

    /**
     * Move assignment operator.
     */
    Exit& operator=(Exit &&) = default;

    /**
     * Callback to let the exit know that another update is in progress. The
     * base implementation does nothing.
     *
     * @param theIntervalInSeconds
     *          time since last update
     */
    virtual void update(float theIntervalInSeconds);

    protected:

    /**
     * Populates myColor from the "color" property.
     *
     * @throws std::invalid_argument on invalid RGB color.
     */
    void setColorFromProperty();

    private:

    /** Exit color, in RGB suitable for OpenGL (each value is 0.0-1.0)*/
    Eigen::Vector3f myColor;
    
  };
}
