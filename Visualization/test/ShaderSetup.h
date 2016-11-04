#pragma once

/**
 * @file ShaderSetup.h
 * @brief Base class for use in testing shader related classes. User needs to
 * #define CLASS_NAME before including this file (this to give a good name for
 * the tests.
 *
 * @author Michael Albers
 */

#include <string>

class CLASS_NAME: public ::testing::Test
{
  public:

  void SetUp()
  {
    glfwSetErrorCallback(&errorCallback);

    QS::VisualizationInitialization::InitializeGLFW();

    glfwWindowHint(GLFW_VISIBLE, 0);

    myLatestError = "";
    myWindow = glfwCreateWindow(1, 1, "DummyWindow", NULL, NULL);
    if (!myWindow)
    {
      std::string error{"Failed to create OpenGL context window: "};
      error += myLatestError;
      throw std::logic_error(error);
    }
    glfwHideWindow(myWindow);
    glfwMakeContextCurrent(myWindow);

    QS::VisualizationInitialization::InitializeGLEW();
  }

  void TearDown()
  {
    if (nullptr == myWindow)
    {
      glfwDestroyWindow(myWindow);
    }
    QS::VisualizationInitialization::TerminateGLEW();
    QS::VisualizationInitialization::TerminateGLFW();
  }

  protected:

  static void errorCallback(int error, const char* description)
  {
    myLatestError = std::string(description) + " (" +
      std::to_string(error) + ")";
  }

  GLFWwindow *myWindow = nullptr;
  static std::string myLatestError;
};

