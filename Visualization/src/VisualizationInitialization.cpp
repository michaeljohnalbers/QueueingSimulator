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
  // According to https://www.opengl.org/wiki/OpenGL_Loading_Library you can
  // get an GL_INVALID_ENUM error when setting glewExperimental to true with
  // certain (and unspecified) versions of GLEW. This clears this error.
  glGetError();
}

void QS::VisualizationInitialization::InitializeGLFW()
{
  if (! glfwInit())
  {
    throw std::logic_error("glfwInit failed!");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void QS::VisualizationInitialization::TerminateGLEW() noexcept
{
  // No GLEW terminate actions.
}

void QS::VisualizationInitialization::TerminateGLFW() noexcept
{
  glfwTerminate();
}

