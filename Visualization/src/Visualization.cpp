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

float QS::Visualization::getAspectRatio() const noexcept
{
  return myAspectRatio;
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
  std::tie(width, height) = getWindowDimensions();

  setWindowHints();
  myWindow = glfwCreateWindow(width, height, "Queueing Simulator", NULL, NULL);
  if (!myWindow)
  {
    throw std::logic_error("Failed to create simulation visualization window.");
  }
  glfwMakeContextCurrent(myWindow);

  VisualizationInitialization::InitializeGLEW();

  glViewport(0, 0, width, height);
  glfwSetWindowAspectRatio(myWindow, width, height);
  glEnable(GL_DEPTH_TEST);

  // Set callbacks. Do derived class first in case it were to set a callback
  // this class depends upon.
  setCallbacks(myWindow);
  glfwSetFramebufferSizeCallback(myWindow, &frameBufferCallback);

  glfwSwapInterval(1);
}

void QS::Visualization::preBufferSwap() noexcept
{
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

void QS::Visualization::setCallbacks(GLFWwindow *theWindow)
{
}

void QS::Visualization::setWindowHints() noexcept
{
}

void QS::Visualization::startThread()
{
  myThread.reset(new std::thread(run, this));
}

void QS::Visualization::stopThread()
{
  // I don't like this (I'd rather it be in the destructor) but the problem is
  // that the BatchVisualization class has dynamically allocated memory and it
  // is getting freed while the thread is still running and this is causing
  // seg faults and other memory issues.
  if (myThread)
  {
    myThreadControl = false;
    myThread->join();
  }
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

  myThreadControl = true;
  while (myThreadControl)
  {
    glfwPollEvents();

    if (myUserInput.size() > 0)
    {
      processUserInput();
    }

    float updateInterval = getUpdateInterval();

    if (0.0 != updateInterval)
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

    preBufferSwap();

    glfwSwapBuffers(myWindow);

#ifdef QS_DEBUG_BUILD // Set during cmake debug build
    GLenum lastGlError = glGetError();
    if (lastGlError != GL_NO_ERROR)
    {
      std::cerr << "Post frame OpenGL error: " << gluErrorString(lastGlError)
                << std::endl;
    }
#endif
  }
}
