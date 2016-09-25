#pragma once

/**
 * @file ShaderSetup.h
 * @brief Base class for use in testing shader related classes. User needs to
 * #define CLASS_NAME before including this file (this to give a good name for
 * the tests.
 *
 * @author Michael Albers
 */

class CLASS_NAME: public ::testing::Test
{
  public:

  void SetUp()
  {
    QS::VisualizationInitialization::InitializeGLFW();

    myWindow = glfwCreateWindow(1, 1, "DummyWindow", NULL, NULL);
    if (!myWindow)
    {
      throw std::logic_error("Failed to create OpenGL context window.");
    }
    glfwHideWindow(myWindow);
    glfwMakeContextCurrent(myWindow);

    QS::VisualizationInitialization::InitializeGLEW();
  }

  void TearDown()
  {
    glfwDestroyWindow(myWindow);
    QS::VisualizationInitialization::TerminateGLFW();
  }

  protected:

  GLFWwindow *myWindow;
};

