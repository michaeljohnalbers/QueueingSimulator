/**
 * @file Visualization.cpp
 * @brief Definition of Visualization
 *
 * @author Michael Albers
 */

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
#include "WorldBox.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Actor.h"// TODO: remove

QS::Visualization::Visualization(float theXDimension_m, float theYDimension_m) :
  myXDimension_m(theXDimension_m),
  myYDimension_m(theYDimension_m)
{
  myAspectRatio = myXDimension_m / myYDimension_m;
}

QS::Visualization::~Visualization()
{
  if (myThread)
  {
    // Wake up GLFW for shutdown.
    glfwPostEmptyEvent();
    myThreadControl = false; // TODO: not a thread-safe way to do this
    myThread->join();
  }
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

  VisualizationInitialization::InitializeGLEW();

  glViewport(0, 0, width, height);
  glfwSetWindowAspectRatio(myWindow, width, height);
  glEnable(GL_DEPTH_TEST);

  // Handle window resize.
  glfwSetFramebufferSizeCallback(myWindow, &frameBufferCallback);

  glfwSwapInterval(1);
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

void QS::Visualization::visualize()
{
  initializeGLFW();

  myActors.reset(new Actors());
  myWorldBox.reset(new WorldBox(myXDimension_m, myYDimension_m));

  // TODO: eventually remove this
  Actor actor({{"mass", "1.0"}, {"radius", "50.0"}});
  actor.setPosition({50.0, 50.0});
  Actor actor2({{"mass", "1.0"}, {"radius", "25.0"}});
  actor2.setPosition({myXDimension_m - 25.0, 25.0});
  std::vector<Actor*> actors{&actor, &actor2};

  // TODO: Absolutely no idea how the near/far values work.
  float zFar = 1000.0;
  float zNear = 0.1;
  glm::mat4 projectionMatrix;
  projectionMatrix = glm::perspective(45.0f, myAspectRatio, zNear, zFar);
 
  //https://www.opengl.org/discussion_boards/showthread.php/171541-glm-Triangle-with-perspective
  // TODO: This will be adjusted when moving the camera.
  glm::mat4 viewMatrix;
  viewMatrix = glm::lookAt(
    glm::vec3(myXDimension_m/2, myYDimension_m/2,
              std::min(myXDimension_m, myYDimension_m)), 
    glm::vec3(myXDimension_m/2, myYDimension_m/2, 0.0f), 
    glm::vec3(0.0f, 1.0f, 0.0f));

  myThreadControl = true;
  while (myThreadControl)
  {
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    myWorldBox->draw(viewMatrix, projectionMatrix);
    myActors->draw(viewMatrix, projectionMatrix, actors);

    glfwSwapBuffers(myWindow);
  }

  glfwDestroyWindow(myWindow);
  VisualizationInitialization::TerminateGLFW();
  glfwSetErrorCallback(0);
}
