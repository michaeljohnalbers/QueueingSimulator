/**
 * @file VisualizationInitialization.cpp
 * @brief Definition of VisualizationInitialization
 *
 * @author Michael Albers
 */

#include <sstream>
#include <stdexcept>
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "VisualizationInitialization.h"

void QS::VisualizationInitialization::InitializeGLEW()
{
  glewExperimental = GL_TRUE; // For glGenVertexArrays to not core
  GLenum glewInitError = glewInit();
  if (GLEW_OK != glewInitError)
  {
    std::ostringstream error;
    error << "GLEW initialization failed: "
          << glewGetErrorString(glewInitError);
    throw std::logic_error(error.str());
  }
}

void QS::VisualizationInitialization::InitializeGLFW()
{
  if (! glfwInit())
  {
    throw std::logic_error("glfwInit failed!");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

void QS::VisualizationInitialization::TerminateGLEW() noexcept
{
  // No GLEW terminate actions.
}

void QS::VisualizationInitialization::TerminateGLFW() noexcept
{
  glfwTerminate();
}

