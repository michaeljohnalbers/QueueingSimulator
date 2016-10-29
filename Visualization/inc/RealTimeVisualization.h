#pragma once

/**
 * @file RealTimeVisualization.h
 * @brief Visualization of the simulation in real-time
 *
 * @author Michael Albers
 */

#include "Visualization.h"

namespace QS
{
  class RealTimeVisualization : public Visualization
  {
    public:

    /**
     * Default constructor.
     */
    RealTimeVisualization() = delete;

    /**
     * Constructor.
     *
     * @param theWorld
     *          simulation world
     */
    RealTimeVisualization(World &theWorld);

    /**
     * Copy constructor
     */
    RealTimeVisualization(const RealTimeVisualization&) = delete;

    /**
     * Move constructor
     */
    RealTimeVisualization(RealTimeVisualization&&) = delete;

    /**
     * Destructor.
     */
    virtual ~RealTimeVisualization() = default;

    /**
     * Copy assignment operator
     */
    RealTimeVisualization& operator=(const RealTimeVisualization&) = delete;

    /**
     * Move assignment operator
     */
    RealTimeVisualization& operator=(RealTimeVisualization&&) = delete;

    protected:

    /**
     * Returns the amount of time for the next update. Also handles pausing the
     * simulation and single-frame stepping.
     *
     * @return update time
     */
    virtual float getUpdateInterval() noexcept override;

    /**
     * Gets the window size for the simulation.
     *
     * @return width/height pair
     */
    virtual std::tuple<int, int> getWindowDimensions() noexcept override;

    /**
     * Sets any callbacks a derived class may need.
     *
     * @param theWindow
     *          window to set callbacks against
     */
    virtual void setCallbacks(GLFWwindow *theWindow) override;

    private:

    /** Maximum frames per second. */
    static constexpr double MAX_FPS = 60.0;

    /** Seconds per frame at max FPS. */
    static constexpr double TIME_PER_FRAME = 1.0/MAX_FPS;

    /**
     * Callback for receiving text input.
     */
    static void charCallback(GLFWwindow* window, unsigned int codepoint);

    /**
     * Callback function for when a mouse button is pressed.
     */
    static void mouseButtonCallback(GLFWwindow* window, int button, int action,
                                    int mods) noexcept;

    /**
     * Member function for handling character input.
     *
     * @param theCodePoint
     *          character input
     */
    void processChar(unsigned int theCodePoint);

    /** Is one simulation update needed? */
    bool myNeedOneUpdate = false;

    /** Saved time for update interval calculation*/
    double mySavedTime;

    /** Window height (pixels) */
    int myWindowHeight;

    /** Window width (pixels) */
    int myWindowWidth;
  };
}
