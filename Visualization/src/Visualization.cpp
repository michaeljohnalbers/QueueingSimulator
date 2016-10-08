/**
 * @file Visualization.cpp
 * @brief Definition of Visualization
 *
 * @author Michael Albers
 */

#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Visualization.h"
#include "VisualizationInitialization.h"
#include "Actors.h"
#include "Finally.h"
#include "World.h"
#include "WorldBox.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

QS::Visualization::Visualization(World &theWorld) :
  myWorld(theWorld)
{
  std::tie(myXDimension_m, myYDimension_m) = myWorld.getDimensions();
  myAspectRatio = myXDimension_m / myYDimension_m;
}

QS::Visualization::~Visualization()
{
  if (myThread)
  {
    myThreadControl = false;
    myThread->join();
  }
}

void QS::Visualization::charCallback(GLFWwindow* window, unsigned int codepoint)
{
  reinterpret_cast<Visualization*>(
    glfwGetWindowUserPointer(window))->processChar(codepoint);
}

void QS::Visualization::errorCallback(int theError, const char *theDescription)
{
  // TODO: probably needs to be more robust
  std::cerr << "GLFW Error! Code: " << theError
            << ". Description: " << theDescription << std::endl;
}

void QS::Visualization::frameBufferCallback(
  GLFWwindow* theWwindow, int theWidth, int theHeight)
{
  glViewport(0, 0, theWidth, theHeight);
}

std::tuple<float, float> QS::Visualization::getCameraPosition() const noexcept
{
  return std::make_tuple(myCameraPosition.x, myCameraPosition.y);
}

float QS::Visualization::getCameraZoom() const noexcept
{
  float zPosition = myCameraPosition.z;
  if (zPosition == 0.0)
  {
    zPosition = 0.00001;
  }
  return (myOriginalZoomDistance / zPosition) * 100.0;
}

std::tuple<int, int> QS::Visualization::getRealTimeWindowDimensions()
  const noexcept
{
  const float scaleFactor = 0.9;
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  float maxHeight = mode->height * scaleFactor;
  float maxWidth = mode->width * scaleFactor;

  // This finds the window size that best fits into the maximums above while
  // still maintaining the aspect ratio of the world. There are probably more
  // efficient ways to do this, but I can't think of one and the loop shouldn't
  // ever be more than a few thousand iterations, which isn't bad at all.
  float width = myAspectRatio;
  float height = 1.0;
  while (width < maxWidth && height < maxHeight)
  {
    height++;
    width = height * myAspectRatio;
  }

  return std::make_tuple(static_cast<int>(width), static_cast<int>(height));
}

QS::Visualization::SimulationState QS::Visualization::getState() const noexcept
{
  return mySimulationState;
}

void QS::Visualization::initializeGLFW()
{
  glfwSetErrorCallback(errorCallback);

  // Not the most efficient to init/terminate for each visualization, but in
  // the overall scheme, compared to the simulation itself it isn't too much of
  // a cost.
  VisualizationInitialization::InitializeGLFW();

  int width, height;
  std::tie(width, height) = getRealTimeWindowDimensions();

  myWindow = glfwCreateWindow(width, height, "Queueing Simulator", NULL, NULL);
  if (!myWindow)
  {
    throw std::logic_error("Failed to create simulation visualization window.");
  }
  glfwMakeContextCurrent(myWindow);

  glfwSetWindowUserPointer(myWindow, this);
  VisualizationInitialization::InitializeGLEW();

  glViewport(0, 0, width, height);
  glfwSetWindowAspectRatio(myWindow, width, height);
  glEnable(GL_DEPTH_TEST);

  // Set callbacks.
  glfwSetFramebufferSizeCallback(myWindow, &frameBufferCallback);
  glfwSetCharCallback(myWindow, charCallback);
  glfwSetMouseButtonCallback(myWindow, mouseButtonCallback);

  glfwSwapInterval(1);
}

void QS::Visualization::mouseButtonCallback(
  GLFWwindow* window, int button, int action, int mods) noexcept
{
  reinterpret_cast<Visualization*>(
    glfwGetWindowUserPointer(window))->processButtonClick(button, action, mods);
}

void QS::Visualization::processButtonClick(
  int theButton, int theAction, int theMods) noexcept
{
  if (theButton == GLFW_MOUSE_BUTTON_LEFT && theAction == GLFW_PRESS)
  {
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    double x, y;
    glfwGetCursorPos(myWindow, &x, &y);
    y = mode->height - y; // unProject expects (0,0) to be lower left, whereas
                          // GLFW has it in upper left.

    // TODO: this doesn't produce anything near the correct results. Will need
    // to work on it further, time permitting.
    glm::vec3 windowCoordinate(x, y, 0.0);
    glm::vec4 viewport(0, 0, mode->width, mode->height);
    glm::vec3 unprojected = glm::unProject(windowCoordinate,
                                           myViewMatrix,
                                           myProjectionMatrix,
                                           viewport);

    // std::cout << "Mouse at: (" << std::fixed << x
    //           << "," << std::fixed << y << ") --> ("
    //           << unprojected.x << "," << unprojected.y << ")"
    //           << std::endl;
  }
}

void QS::Visualization::processChar(unsigned int theCodePoint)
{
  switch (theCodePoint)
  {
    case 32: // Space
      if (SimulationState::PAUSED == mySimulationState)
      {
        myNeedOneUpdate = true;
      }
      break;

    default:
      break;
  }
}

void QS::Visualization::processUserInput() noexcept
{
  std::lock_guard<std::mutex> guard(myUserInputMutex);
  // TODO: make this user-configurable
  float cameraMoveFactor = 0.005;

  while (! myUserInput.empty())
  {
    UserInput userInput = myUserInput.front();
    myUserInput.pop();

    switch (userInput.myType)
    {
      case UserInputType::PAUSE:
        mySimulationState = SimulationState::PAUSED;
        break;

      case UserInputType::PLAY:
        mySimulationState = SimulationState::RUNNING;
        break;

      case UserInputType::CAMERA_RIGHT:
        cameraMoveFactor *= -1.0;
        // Intentionally leaving out the break.
      case UserInputType::CAMERA_LEFT:
      {
        float moveAmount = myXDimension_m * cameraMoveFactor;
        myCameraPosition = glm::vec3(myCameraPosition.x + moveAmount,
                                     myCameraPosition.y,
                                     myCameraPosition.z);
        myCameraCenter = glm::vec3(myCameraCenter.x + moveAmount,
                                   myCameraCenter.y,
                                   myCameraCenter.z);
      }
      break;

      case UserInputType::CAMERA_UP:
        cameraMoveFactor *= -1.0;
        // Intentionally leaving out the break.
      case UserInputType::CAMERA_DOWN:
      {
        float moveAmount = myYDimension_m * cameraMoveFactor;
        myCameraPosition = glm::vec3(myCameraPosition.x,
                                     myCameraPosition.y + moveAmount,
                                     myCameraPosition.z);
        myCameraCenter = glm::vec3(myCameraCenter.x,
                                   myCameraCenter.y + moveAmount,
                                   myCameraCenter.z);
      }
      break;

      case UserInputType::ZOOM_IN:
        cameraMoveFactor *= -1.0;
        // Intentionally leaving out the break.
      case UserInputType::ZOOM_OUT:
      {
        float moveAmount = std::min(myXDimension_m, myYDimension_m) *
          cameraMoveFactor;
        myCameraPosition = glm::vec3(myCameraPosition.x,
                                     myCameraPosition.y,
                                     myCameraPosition.z + moveAmount);
        myCameraCenter = glm::vec3(myCameraCenter.x,
                                   myCameraCenter.y,
                                   myCameraCenter.z + moveAmount);
      }
      break;

      default:
        break;
    }
  }
}

void QS::Visualization::run(Visualization *theVisualizer) noexcept
{
  try
  {
    theVisualizer->visualize();
  }
  catch (const std::exception &exception)
  {
    std::cerr << "Fatal error: " << exception.what()
              << ". Aborting simulation." << std::endl;
  }
}

void QS::Visualization::startThread()
{
  myThread.reset(new std::thread(run, this));
}

void QS::Visualization::userInput(UserInputType theInputType) noexcept
{
  std::lock_guard<std::mutex> guard{myUserInputMutex};
  myUserInput.push({theInputType, 0.0});
}

void QS::Visualization::userInput(UserInputType theInputType, float theData)
  noexcept
{
  std::lock_guard<std::mutex> guard{myUserInputMutex};
  myUserInput.push({theInputType, theData});
}

void QS::Visualization::visualize()
{
  initializeGLFW();
  Finally terminate(
    [=]()
    {
      if (myWindow)
      {
        glfwDestroyWindow(myWindow);
      }
      VisualizationInitialization::TerminateGLEW();
      VisualizationInitialization::TerminateGLFW();
      glfwSetErrorCallback(0);
    });

  myActors.reset(new Actors());
  myWorldBox.reset(new WorldBox(myXDimension_m, myYDimension_m));

  const std::vector<Actor*> &actors = myWorld.getActors();

  // TODO: Absolutely no idea how the near/far values work.
  float zFar = 1000.0;
  float zNear = 0.1;
  myProjectionMatrix = glm::perspective(45.0f, myAspectRatio, zNear, zFar);

  // Sets initial camera position to middle of the world.
  myOriginalZoomDistance = std::min(myXDimension_m, myYDimension_m);
  myCameraPosition = glm::vec3(myXDimension_m/2, myYDimension_m/2,
                               myOriginalZoomDistance);
  myCameraCenter = glm::vec3(myXDimension_m/2, myYDimension_m/2, 0.0f);

  // Constraining the simulation to a maximum FPS does two things. First, it
  // uses less CPU when running smaller simulations. Second, for smaller
  // simulations the update interval can be microscopic, or zero, when measured
  // in milliseconds (as the former implementation did). A zero update can
  // cause issues with the update math as can be imagined.
  double savedTime = glfwGetTime();
  constexpr double MAX_FPS = 60.0;
  constexpr double TIME_PER_FRAME = 1.0/MAX_FPS;

  myThreadControl = true;
  while (myThreadControl)
  {
    glfwPollEvents();

    if (myUserInput.size() > 0)
    {
      processUserInput();
    }

    double currentTime = glfwGetTime();
    double updateInterval = TIME_PER_FRAME;
    bool doUpdate = false;
    if (SimulationState::RUNNING == mySimulationState)
    {
      updateInterval = currentTime - savedTime;
      if (updateInterval >= TIME_PER_FRAME)
      {
        doUpdate = true;
        savedTime = currentTime;
      }
      else
      {
        double timeToSleep = TIME_PER_FRAME - updateInterval;
        timespec sleepTimeStruct;
        sleepTimeStruct.tv_sec = 0;
        sleepTimeStruct.tv_nsec = static_cast<long>(timeToSleep * 1000000000);
        ::nanosleep(&sleepTimeStruct, 0);
      }
    }
    else
    {
      savedTime = currentTime;
      if (myNeedOneUpdate)
      {
        doUpdate = true;
        myNeedOneUpdate = false;
      }
      else
      {
        // Avoid spinning when paused.
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }
    }

    if (doUpdate)
    {
      myThreadControl = (! myWorld.update(static_cast<float>(updateInterval)));
    }


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //https://www.opengl.org/discussion_boards/showthread.php/171541-glm-Triangle-with-perspective
    myViewMatrix = glm::lookAt(myCameraPosition, myCameraCenter, 
                               glm::vec3(0.0f, 1.0f, 0.0f));

    myWorldBox->draw(myViewMatrix, myProjectionMatrix);
    myActors->draw(myViewMatrix, myProjectionMatrix, actors);

    glfwSwapBuffers(myWindow);
  }
}
