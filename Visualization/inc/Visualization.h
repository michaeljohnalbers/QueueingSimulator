#pragma once

/**
 * @file Visualization.h
 * @brief Visualization class
 *
 * @author Michael Albers
 */

#include <memory>
#include <thread>

#include <GL/glew.h>
#include "GLFW/glfw3.h"

namespace QS
{
  class Visualization
  {
    public:

    /**
     * Default constructor.
     */
    Visualization() = delete;

    /**
     * Constructor.
     *
     * @param theXDimension_m
     *          size of the X dimension, in meters
     * @param theYDimension_m
     *          size of the Y dimension, in meters
     */
    Visualization(float theXDimension_m, float theYDimension_m);

    /**
     * Copy constructor
     */
    Visualization(const Visualization&) = delete;

    /**
     * Move constructor
     */
    Visualization(Visualization&&) = delete;

    /**
     * Destructor.
     */
    ~Visualization();

    /**
     * Copy assignment operator
     */
    Visualization& operator=(const Visualization&) = delete;

    /**
     * Move assignment operator
     */
    Visualization& operator=(Visualization&&) = delete;

    /**
     * Starts the visualization thread.
     */
    void startThread();

    protected:

    /**
     * Thread run function.
     *
     * @param theVisualizer
     *          visualizer to use in the thread.
     */
    static void run(Visualization *theVisualizer) noexcept;

    private:

    /**
     * GLFW error callback
     */
    static void errorCallback(int theError, const char *theDescription);

    /**
     * Callback for window frame buffer resize (pixel based numbers for window
     * resize.
     */
    static void frameBufferCallback(GLFWwindow* theWwindow, int theWidth,
                                    int theHeight);

    /**
     * Gets the window sizes for real-time simulations (when the user will
     * actually see the data on their screen).
     *
     * @return width/height pair
     */
    std::tuple<int, int> getRealTimeWindowDimensions() const noexcept;

    /**
     * Creates everything needed to draw the world box.
     */
    void initWorldBox();

    /**
     * Visualization main loop
     */
    void visualize();

    /** Visualization thread. */
    std::shared_ptr<std::thread> myThread;

    /** Thread control variable */
    bool myThreadControl = false;

    /** Main visualization window. */
    GLFWwindow *myWindow = nullptr;

    /** Buffer for world box points. */
    GLuint myWorldBoxBuffer;

    /** World box vertex array object. */
    GLuint myWorldVAO[2];

    /** Size of the X dimension, meters. */
    const float myXDimension_m;

    /** x / y, used for scaling X dimenions. */
    float myXScale;

    /** Size of Y dimension, meters. */
    const float myYDimension_m;

    /** y / x, used for scaling Y dimensions */
    float myYScale;
  };
}
