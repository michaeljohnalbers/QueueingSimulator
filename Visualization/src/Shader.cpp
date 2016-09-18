/**
 * @file Shader.cpp
 * @brief Definition of Shader
 *
 * @author Michael Albers
 */

#include <cstring>
#include <sstream>
#include <stdexcept>
#include "Shader.h"

QS::Shader::Shader(const std::string &theSource,
                   GLenum theType)
{
  myShader = glCreateShader(theType);
  if (0 == myShader)
  {
    auto error = glGetError();
    throw std::logic_error("Failed to create OpenGL shader. Error " +
                           std::to_string(error));
  }

  loadSource(theSource);
  compile();
}

QS::Shader::~Shader()
{
  glDeleteShader(myShader);
}

void QS::Shader::compile()
{
  glCompileShader(myShader);
  GLint compileStatus;
  glGetShaderiv(myShader, GL_COMPILE_STATUS, &compileStatus);
  if (GL_FALSE == compileStatus)
  {
    std::ostringstream errors;
    errors << "Failed to compile shader.";

    GLint errorLogSize = 0;
    glGetShaderiv(myShader, GL_INFO_LOG_LENGTH, &errorLogSize);
    if (errorLogSize > 0)
    {
      GLchar* logData = new GLchar[errorLogSize+1];
      glGetShaderInfoLog(myShader, errorLogSize, &errorLogSize, logData);
      errors << " Errors: " << logData;
      delete [] logData;
    }

    throw std::logic_error(errors.str());
  }
}

GLuint QS::Shader::getShader() const noexcept
{
  return myShader;
}

void QS::Shader::loadSource(const std::string &theSource) noexcept
{
  GLint length = theSource.size();
  GLchar *source = new GLchar[length + 1];
  const GLchar *sourceConst = source; // Stupid OpenGL.
  std::strcpy(source, theSource.c_str());
  glShaderSource(myShader, 1, &sourceConst, &length);
  delete [] source;
}

QS::Shader::operator GLuint() const noexcept
{
  return getShader();
}
