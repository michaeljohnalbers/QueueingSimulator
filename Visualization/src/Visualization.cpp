/**
 * @file Visualization.cpp
 * @brief Definition of Visualization
 *
 * @author Michael Albers
 */

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Visualization.h"
#include "VisualizationInitialization.h"

QS::Visualization::Visualization(float theXDimension_m, float theYDimension_m) :
  myXDimension_m(theXDimension_m),
  myYDimension_m(theYDimension_m)
{
  myXScale = myXDimension_m / myYDimension_m;
  myYScale = myYDimension_m / myXDimension_m;

  if (myXScale > myYScale)
  {
    myXScale = 1.0;
  }
  else
  {
    myYScale = 1.0;
  }
}

QS::Visualization::~Visualization()
{
  if (myThread)
  {
    myThreadControl = false;
    // Wake up GLFW for shutdown.
    glfwPostEmptyEvent();
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
  // Size window to 3/4 of height (assumes height is smaller of the dimensions,
  // or at least equal to width, which is a safe assumption for pretty much all
  // monitors any more as far as I know) with width keeping world aspect ratio
  // consisten.
  const float scaleFactor = 0.75;
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  float maxHeight = mode->height * scaleFactor;
  float maxWidth = mode->width * scaleFactor;

  float width = myXScale;
  float height = myYScale;

  // Finds the window size that maintains the aspect ratio of the world AND
  // making sure it expands to either maxHeight or maxWidth without going over
  // either. There is probably some clever math to do this, but I tried for
  // hours to figure it out and couldn't do it. This loop should be pretty fast
  // since it will only go to 0.75 of the smallest screen dimension.
  while (width < maxWidth && height < maxHeight)
  {
    if (1.0 == myYScale)
    {
      height++;
      width = height * myXScale;
    }
    else
    {
      width++;
      height = width * myYScale;
    }
  }
  return std::make_tuple(static_cast<int>(width), static_cast<int>(height));
}

void QS::Visualization::initWorldBox()
{
  glGenVertexArrays(2, myWorldVAO);
  glBindVertexArray(myWorldVAO[0]);

  // Origin is at middle of screen
  constexpr int NUM_POINTS = 4;
  GLfloat worldBoxPoints[][2] = {
    {-0.9, 0.9}, {0.9, 0.9},
    {0.9, -0.9}, {-0.9, -0.9}};

  for (int pointIndex = 0; pointIndex < NUM_POINTS; ++pointIndex)
  {
    worldBoxPoints[pointIndex][0] *= myXScale;
    worldBoxPoints[pointIndex][1] *= myYScale;
  }

  glGenBuffers(1, &myWorldBoxBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, myWorldBoxBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(worldBoxPoints), worldBoxPoints,
               GL_STATIC_DRAW);

  // TODO: change to use glGetAttribLocation
  const int vPositionLocation = 0;
  glVertexAttribPointer(vPositionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPositionLocation);
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

  glfwSetWindowAspectRatio(myWindow, width, height);

  glfwMakeContextCurrent(myWindow);
  VisualizationInitialization::InitializeGLEW();

  // Handle window resize.
  glfwSetFramebufferSizeCallback(myWindow, &frameBufferCallback);

  glfwSwapInterval(1);

  std::string passThroughVertexShaderSource = 
#include "PassThrough.vert"
    ;

  std::string redFragmentShaderSource = 
#include "Red.frag"
    ;

  Shader passThroughVertexShader(passThroughVertexShaderSource,
                                 GL_VERTEX_SHADER);
  Shader redFragmentShader(redFragmentShaderSource,
                           GL_FRAGMENT_SHADER);
  ShaderProgram worldBoxProgram;
  worldBoxProgram.attachShader(passThroughVertexShader);
  worldBoxProgram.attachShader(redFragmentShader);
  worldBoxProgram.link();

  initWorldBox();

  myThreadControl = true;
  while (myThreadControl)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    worldBoxProgram.use();

    glBindVertexArray(myWorldVAO[0]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glfwSwapBuffers(myWindow);

    glfwPollEvents();
  }

  glfwDestroyWindow(myWindow);
  VisualizationInitialization::TerminateGLFW();
  glfwSetErrorCallback(0);
}
