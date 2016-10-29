/**
 * @file RealTimeVisualization.cpp
 * @brief Definition of RealTimeVisualization
 *
 * @author Michael Albers
 */

#include "RealTimeVisualization.h"
#include "glm/gtc/matrix_transform.hpp"

QS::RealTimeVisualization::RealTimeVisualization(World &theWorld) :
  Visualization(theWorld),
  mySavedTime(glfwGetTime())
{
}

void QS::RealTimeVisualization::charCallback(
  GLFWwindow* window, unsigned int codepoint)
{
  reinterpret_cast<RealTimeVisualization*>(
    glfwGetWindowUserPointer(window))->processChar(codepoint);
}

float QS::RealTimeVisualization::getUpdateInterval() noexcept
{
  // Constraining the simulation to a maximum FPS does two things. First, it
  // uses less CPU when running smaller simulations. Second, for smaller
  // simulations the update interval can be microscopic, or zero, when measured
  // in milliseconds (as the former implementation did). A zero update can
  // cause issues with the update math as can be imagined.

  double currentTime = glfwGetTime();
  double updateInterval = TIME_PER_FRAME;
  if (SimulationState::RUNNING == getState())
  {
    updateInterval = currentTime - mySavedTime;
    if (updateInterval >= TIME_PER_FRAME)
    {
      mySavedTime = currentTime;
    }
    else
    {
      double timeToSleep = TIME_PER_FRAME - updateInterval;
      timespec sleepTimeStruct;
      sleepTimeStruct.tv_sec = 0;
      sleepTimeStruct.tv_nsec = static_cast<long>(timeToSleep * 1000000000);
      ::nanosleep(&sleepTimeStruct, 0);

      // No frame update this time around.
      updateInterval = 0.0;
    }
    // Don't "buffer" an update (if user hits space while simulation is running
    // that will advance the simulation one frame when the user hits pause.
    myNeedOneUpdate = false;
  }
  else
  {
    mySavedTime = currentTime;
    if (myNeedOneUpdate)
    {
      myNeedOneUpdate = false;
    }
    else
    {
      // Avoid spinning when paused.
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      updateInterval = 0.0;
    }
  }

  return updateInterval;
}

std::tuple<int, int> QS::RealTimeVisualization::getWindowDimensions()
  noexcept
{
  const float scaleFactor = 0.9;
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  float maxHeight = mode->height * scaleFactor;
  float maxWidth = mode->width * scaleFactor;

  // This finds the window size that best fits into the maximums above while
  // still maintaining the aspect ratio of the world. There are probably more
  // efficient ways to do this, but I can't think of one and the loop shouldn't
  // ever be more than a few thousand iterations, which isn't bad at all.
  float aspectRatio = getAspectRatio();
  float width = aspectRatio;
  float height = 1.0;
  while (width < maxWidth && height < maxHeight)
  {
    height++;
    width = height * aspectRatio;
  }

  myWindowWidth = static_cast<int>(width);
  myWindowHeight = static_cast<int>(height);

  return std::make_tuple(myWindowWidth, myWindowHeight);
}

void QS::RealTimeVisualization::mouseButtonCallback(
  GLFWwindow* window, int button, int action, int mods) noexcept
{
  RealTimeVisualization *visualization =
    reinterpret_cast<RealTimeVisualization*>(glfwGetWindowUserPointer(window));

  if (GLFW_MOUSE_BUTTON_LEFT == button && GLFW_PRESS == action)
  {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    // unProject expects (0,0) to be lower left, whereas GLFW has it in upper
    // left.
    y = visualization->myWindowHeight - y;

    // TODO: this doesn't produce anything near the correct results. Will need
    // to work on it further, time permitting.
    glm::vec3 windowCoordinate(x, y, 0.0);
    glm::vec4 viewport(
      0, 0, visualization->myWindowWidth, visualization->myWindowHeight);
    glm::vec3 unprojected = glm::unProject(windowCoordinate,
                                           visualization->getViewMatrix(),
                                           visualization->getProjectionMatrix(),
                                           viewport);

    unprojected = unprojected;

    // std::cout << "Mouse at: (" << std::fixed << x
    //           << "," << std::fixed << y << ") --> ("
    //           << unprojected.x << "," << unprojected.y << ")"
    //           << std::endl;
  }
}

void QS::RealTimeVisualization::processChar(unsigned int theCodePoint)
{
  switch (theCodePoint)
  {
    case 32: // Space
      if (SimulationState::PAUSED == getState())
      {
        myNeedOneUpdate = true;
      }
      break;

    default:
      break;
  }
}

void QS::RealTimeVisualization::setCallbacks(GLFWwindow *theWindow)
{
  glfwSetWindowUserPointer(theWindow, this);
  glfwSetCharCallback(theWindow, charCallback);
  glfwSetMouseButtonCallback(theWindow, mouseButtonCallback);
}
