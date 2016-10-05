#pragma once

/**
 * @file Visualization.h
 * @brief Visualization class
 *
 * @author Michael Albers
 */

#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace QS
{
  class Actors;
  class ShaderProgram;
  class World;
  class WorldBox;

  class Visualization
  {
    public:

    /** Input types from the user. */
    enum class UserInputType
    {
      PLAY,
      PAUSE,
      CAMERA_LEFT,
      CAMERA_RIGHT,
      CAMERA_UP,
      CAMERA_DOWN,
      ZOOM_IN,
      ZOOM_OUT
    };

    /** State of the simulation. */
    enum class SimulationState
    {
      RUNNING,
      PAUSED
    };

    /**
     * Default constructor.
     */
    Visualization() = delete;

    /**
     * Constructor.
     *
     * @param theWorld
     *          simulation world
     */
    Visualization(World &theWorld);

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
     * Returns the camera's position.
     *
     * @return x, y coordinates of the camera
     */
    std::tuple<float, float> getCameraPosition() const noexcept;

    /**
     * Returns the zoom percentage.
     *
     * @return zoom percentage
     */
    float getCameraZoom() const noexcept;

    /**
     * Returns the current state of the simulation.
     *
     * @return the current state of the simulation.
     */
    SimulationState getState() const noexcept;

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

    /**
     * Means to send user input to the visualization.
     *
     * @param theInput
     *          type of input
     */
    void userInput(UserInputType theInputType) noexcept;

    /**
     * Overload of userInput that allows the user to pass arbitrary float data.
     * The data is dependent on the input type.
     * 
     * @param theInput
     *          type of input
     * @param theData
     *          data for input
     */
    void userInput(UserInputType theInputType, float theData) noexcept;

    protected:

    /**
     * Thread run function.
     *
     * @param theVisualizer
     *          visualizer to use in the thread.
     */
    static void run(Visualization *theVisualizer) noexcept;

    private:

    class UserInput
    {
      public:
      UserInputType myType;
      float myData;
    };

    /**
     * Callback for receiving text input.
     */
    static void charCallback(GLFWwindow* window, unsigned int codepoint);

    /**
     * GLFW error callback
     */
    static void errorCallback(int theError, const char *theDescription);

    /**
     * Callback for window frame buffer resize (pixel based numbers for window
     * resize.
     */
    static void frameBufferCallback(GLFWwindow* theWindow, int theWidth,
                                    int theHeight);

    /**
     * Gets the window sizes for real-time simulations (when the user will
     * actually see the data on their screen).
     *
     * @return width/height pair
     */
    std::tuple<int, int> getRealTimeWindowDimensions() const noexcept;

    /**
     * Performs GLFW initialization (creeating window, etc.).
     */
    void initializeGLFW();

    /**
     * Callback function for when a mouse button is pressed.
     */
    static void mouseButtonCallback(GLFWwindow* window, int button, int action,
                                    int mods) noexcept;

    /**
     * Member function to handle button clicks.
     *
     * @param theButton
     *          button clicked
     * @param theAction
     *          action done on the button
     * @param theMods
     *          any modifiers
     */
    void processButtonClick(int theButton, int thAction, int theMods) noexcept;

    /**
     * Member function for handling character input.
     *
     * @param theCodePoint
     *          character input
     */
    void processChar(unsigned int theCodePoint);

    /**
     * Processes all pending user input.
     */
    void processUserInput() noexcept;

    /**
     * Visualization main loop
     */
    void visualize();

    /** Object for drawing actors. */
    std::unique_ptr<Actors> myActors;

    /** width:height of the world, used as window aspect ratio */
    float myAspectRatio;

    /** Position of the camera. */
    glm::vec3 myCameraPosition;

    /** Position the camera is looking at. */
    glm::vec3 myCameraCenter;

    /** Is one simulation update needed? */
    bool myNeedOneUpdate = false;

    /** Original Z position of camera. */
    float myOriginalZoomDistance;

    /** Projection matrix. */
    glm::mat4 myProjectionMatrix;

    /** State of the simulation. */
    SimulationState mySimulationState = SimulationState::RUNNING;

    /** Visualization thread. */
    std::shared_ptr<std::thread> myThread;

    /** Thread control variable */
    bool myThreadControl = false;

    /** User input queue. */
    std::queue<UserInput> myUserInput;

    /** Mutex for thread-safety when accessing myUserInput */
    std::mutex myUserInputMutex;

    /** View matrix. */
    glm::mat4 myViewMatrix;

    /** Main visualization window. */
    GLFWwindow *myWindow = nullptr;

    /** Simulation world. */
    World &myWorld;

    /** Box bounding the world. */
    std::unique_ptr<WorldBox> myWorldBox;

    /** Size of the X dimension, meters. */
    float myXDimension_m;

    /** Size of Y dimension, meters. */
    float myYDimension_m;
  };
}
