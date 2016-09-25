/**
 * @file ShaderProgram.cpp
 * @brief Definition of ShaderProgram
 *
 * @author Michael Albers
 */

#include <sstream>
#include <stdexcept>
#include <string>
#include "Shader.h"
#include "ShaderProgram.h"

QS::ShaderProgram::ShaderProgram() noexcept
{
  myProgram = glCreateProgram();
  if (0 == myProgram)
  {
    auto error = glGetError();
    throw std::logic_error("Failed to create OpenGL shader program. Error " +
                           std::to_string(error) + ".");
  }
}

QS::ShaderProgram::~ShaderProgram()
{
  glDeleteProgram(myProgram);
}

void QS::ShaderProgram::attachShader(Shader &theShader) noexcept
{
  glAttachShader(myProgram, theShader);
}

void QS::ShaderProgram::detachShader(Shader &theShader) noexcept
{
  glDetachShader(myProgram, theShader);
}

GLuint QS::ShaderProgram::getProgram() const noexcept
{
  return myProgram;
}

void QS::ShaderProgram::link()
{
  glLinkProgram(myProgram);
  GLint linkStatus;
  glGetProgramiv(myProgram, GL_LINK_STATUS, &linkStatus);
  if (GL_FALSE == linkStatus)
  {
    std::ostringstream errors;
    errors << "Failed to link shader program.";

    GLint errorLogSize = 0;
    glGetProgramiv(myProgram, GL_INFO_LOG_LENGTH, &errorLogSize);
    if (errorLogSize > 0)
    {
      GLchar* logData = new GLchar[errorLogSize+1];
      glGetProgramInfoLog(myProgram, errorLogSize, &errorLogSize, logData);
      errors << " Errors: " << logData;
      delete [] logData;
    }

    throw std::logic_error(errors.str());
  }
}

QS::ShaderProgram::operator GLuint() const noexcept
{
  return getProgram();
}

void QS::ShaderProgram::use() const noexcept
{
  glUseProgram(myProgram);
}
