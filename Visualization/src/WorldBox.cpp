/**
 * @file WorldBox.cpp
 * @brief Definition of WorldBox
 *
 * @author Michael Albers
 */

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "WorldBox.h"

QS::WorldBox::WorldBox(float theXDimension_m, float theYDimension_m) :
  myXDimension_m(theXDimension_m),
  myYDimension_m(theYDimension_m)
{
  glGenVertexArrays(1, &myVAO);
  glGenBuffers(1, &myVBO);
  glBindVertexArray(myVAO);

  GLfloat vertices[] = {
    -0.5,  0.5, 0.0,  1.0, 1.0, 0.0,
     0.5,  0.5, 0.0,  1.0, 1.0, 0.0,
     0.5, -0.5, 0.0,  1.0, 1.0, 0.0,
    -0.5, -0.5, 0.0,  1.0, 1.0, 0.0
  };

  glBindBuffer(GL_ARRAY_BUFFER, myVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  createShader();

  GLint positionLocation = glGetAttribLocation(myShaderProgram, "inPosition");
  glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE,
                        6 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(positionLocation);

  GLint colorLocation = glGetAttribLocation(myShaderProgram, "inColor");
  glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE,
                        6 * sizeof(GLfloat),
                        reinterpret_cast<void*>(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(colorLocation);

  glBindVertexArray(0);

  myModelMatrix = glm::translate(
    myModelMatrix, glm::vec3(myXDimension_m/2, myYDimension_m/2, 0.0f));
  myModelMatrix = glm::scale(
    myModelMatrix, glm::vec3(myXDimension_m, myYDimension_m, 1.0f));
}

QS::WorldBox::~WorldBox()
{
  glDeleteVertexArrays(1, &myVAO);
  glDeleteBuffers(1, &myVBO);
}

void QS::WorldBox::createShader()
{
  std::string vertexShaderSource =
    #include "WorldBox.vert"
    ;

  std::shared_ptr<Shader> vertexShader{
    new Shader(vertexShaderSource, GL_VERTEX_SHADER)};
  myShaders.push_back(vertexShader);

  std::string fragmentShaderSource =
    #include "WorldBox.frag"
    ;

  std::shared_ptr<Shader> fragmentShader{
    new Shader(fragmentShaderSource, GL_FRAGMENT_SHADER)};
  myShaders.push_back(fragmentShader);

  myShaderProgram.attachShader(*vertexShader);
  myShaderProgram.attachShader(*fragmentShader);
  myShaderProgram.link();
}

void QS::WorldBox::draw(glm::mat4 &theViewMatrix,
                        glm::mat4 &theProjectionMatrix)
{
  glBindVertexArray(myVAO);

  myShaderProgram.use();

  // Have to calculate per draw in case the camera moved. Don't calculate in
  // the shader as it will be the same for every point.
  glm::mat4 modelViewProjectionMatrix =
    theProjectionMatrix * (theViewMatrix * myModelMatrix);

  GLint mvpLocation = glGetUniformLocation(myShaderProgram,
                                           "ModelViewProjection");
  glUniformMatrix4fv(mvpLocation, 1, GL_FALSE,
                     glm::value_ptr(modelViewProjectionMatrix));

  glDrawArrays(GL_LINE_LOOP, 0, 4);
  glBindVertexArray(0);
  glUseProgram(0);
}
