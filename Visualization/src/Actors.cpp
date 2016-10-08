/**
 * @file Actors.cpp
 * @brief Definition of Actors
 *
 * @author Michael Albers
 */

#include <array>
#define _USE_MATH_DEFINES // For M_PI
#include <cmath>
#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Actor.h"
#include "Actors.h"
#include "Shader.h"

QS::Actors::Actors()
{
  glGenVertexArrays(1, &myVAO);
  glGenBuffers(1, &myCircleVertexBuffer);
  glBindVertexArray(myVAO);

  constexpr GLsizei NUM_VERTICES =
    NUM_CIRCLE_VERTICES + NUM_ORIENTATION_IND_VERTICES;

  GLfloat vertices[NUM_VERTICES * 3];

  // === Circle vertices
  int index = 0;
  vertices[index] = 0.0;
  vertices[++index] = 0.0;
  vertices[++index] = 0.0;
  ++index;

#define RADIANS(x) ((x) * M_PI / 180.0)

  for (int degree = 0; degree < NUM_CIRCLE_VERTICES - 1; ++degree, index += 3)
  {
    float radians = RADIANS(degree);
    vertices[index] = std::cos(radians);
    vertices[index+1] = std::sin(radians);
    vertices[index+2] = 0.0;
  }

  // === Orientation indicator vertices
  constexpr float degreeOffset = 3.0;
  vertices[index] = std::cos(RADIANS(degreeOffset));   // x
  vertices[++index] = std::sin(RADIANS(degreeOffset)); // y
  vertices[++index] = 0.0; // z

  vertices[++index] = 1.07;
  vertices[++index] = 0.0;
  vertices[++index] = 0.0;

  vertices[++index] = std::cos(RADIANS(360 - degreeOffset));
  vertices[++index] = std::sin(RADIANS(360 - degreeOffset));
  vertices[++index] = 0.0;

  glBindBuffer(GL_ARRAY_BUFFER, myCircleVertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

  createShader();

  GLint positionLocation = glGetAttribLocation(myShaderProgram, "inPosition");
  glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(positionLocation);

  glBindVertexArray(0);
}

QS::Actors::~Actors()
{
  glDeleteVertexArrays(1, &myVAO);
  glDeleteBuffers(1, &myCircleVertexBuffer);
}

void QS::Actors::createShader()
{
  std::string vertexShaderSource =
    #include "Actor.vert"
    ;

  std::shared_ptr<Shader> vertexShader{
    new Shader(vertexShaderSource, GL_VERTEX_SHADER)};
  myShaders.push_back(vertexShader);

  std::string fragmentShaderSource =
    #include "Actor.frag"
    ;

  std::shared_ptr<Shader> fragmentShader{
    new Shader(fragmentShaderSource, GL_FRAGMENT_SHADER)};
  myShaders.push_back(fragmentShader);

  myShaderProgram.attachShader(*vertexShader);
  myShaderProgram.attachShader(*fragmentShader);
  myShaderProgram.link();
}

void QS::Actors::draw(glm::mat4 &theViewMatrix,
                      glm::mat4 &theProjectionMatrix,
                      const std::vector<Actor*> &theActors)
{
  glBindVertexArray(myVAO);
  myShaderProgram.use();

  GLint viewLocation = glGetUniformLocation(myShaderProgram, "inViewMatrix");
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(theViewMatrix));

  GLint projectionLocation = glGetUniformLocation(myShaderProgram,
                                                  "inProjectionMatrix");
  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE,
                     glm::value_ptr(theProjectionMatrix));

  glm::vec3 *colorVectors = new glm::vec3[theActors.size()];
  glm::mat4 *modelMatrices = new glm::mat4[theActors.size()];

  constexpr glm::vec3 rotationAxis(0.0, 0.0, 1.0);

  for (auto ii = 0u; ii < theActors.size(); ++ii)
  {
    Actor &actor = *theActors[ii];

    // === Color
    Eigen::Vector3f color = actor.getColor();
    // x == r, y == g, z == b
    colorVectors[ii] = glm::vec3(color.x(), color.y(), color.z());

    // === Position
    float actorRadius = actor.getRadius();
    Eigen::Vector2f position = actor.getPosition();
    float orientation = actor.getOrientation();

    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix,
                                 glm::vec3(position.x(), position.y(), 0.0));
    modelMatrix = glm::scale(
      modelMatrix, glm::vec3(actorRadius, actorRadius, 1.0));

    modelMatrix = glm::rotate(modelMatrix, orientation, rotationAxis);

    modelMatrices[ii] = modelMatrix;
  }

  // === Color
  GLuint colorVectorsBuffer;
  glGenBuffers(1, &colorVectorsBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorVectorsBuffer);
  glBufferData(GL_ARRAY_BUFFER, theActors.size() * sizeof(glm::vec3),
               &colorVectors[0], GL_STREAM_DRAW);

  GLint colorLocation = glGetAttribLocation(myShaderProgram, "inColor");
  glEnableVertexAttribArray(colorLocation);
  glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribDivisor(colorLocation, 1);


  // === Position
  GLuint modelMatricesBuffer;
  glGenBuffers(1, &modelMatricesBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, modelMatricesBuffer);
  glBufferData(GL_ARRAY_BUFFER, theActors.size() * sizeof(glm::mat4),
               &modelMatrices[0], GL_STREAM_DRAW);

  GLint modelLocation = glGetAttribLocation(myShaderProgram, "inModelMatrix");

  constexpr GLsizei vec4Size = sizeof(glm::vec4);
  glEnableVertexAttribArray(modelLocation);
  glVertexAttribPointer(modelLocation, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, 0);
  glVertexAttribDivisor(modelLocation, 1);

  modelLocation++;
  glEnableVertexAttribArray(modelLocation);
  glVertexAttribPointer(modelLocation, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                        reinterpret_cast<GLvoid*>(vec4Size));
  glVertexAttribDivisor(modelLocation, 1);

  modelLocation++;
  glEnableVertexAttribArray(modelLocation);
  glVertexAttribPointer(modelLocation, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                        reinterpret_cast<GLvoid*>(2 * vec4Size));
  glVertexAttribDivisor(modelLocation, 1);

  modelLocation++;
  glEnableVertexAttribArray(modelLocation);
  glVertexAttribPointer(modelLocation, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                        reinterpret_cast<GLvoid*>(3 * vec4Size));
  glVertexAttribDivisor(modelLocation, 1);

  glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, NUM_CIRCLE_VERTICES,
                        theActors.size());

  glDrawArraysInstanced(GL_LINE_LOOP, NUM_CIRCLE_VERTICES,
                        NUM_ORIENTATION_IND_VERTICES,
                        theActors.size());

  glBindVertexArray(0);
  glUseProgram(0);
  delete [] colorVectors;
  delete [] modelMatrices;
}
