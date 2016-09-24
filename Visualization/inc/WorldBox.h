#pragma once

/**
 * @file WorldBox.h
 * @brief Draws the world boundaries as a box.
 *
 * @author Michael Albers
 */

#include <memory>
#include <vector>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "ShaderProgram.h"

namespace QS
{
  class Shader;

  /**
   * This class encapsulates everything involved with drawing the world boundary
   * box.
   */
  class WorldBox
  {
    public:

    /**
     * Default constructor
     */
    WorldBox() = delete;

    /**
     * Constructor.
     *
     * @param theXDimension_m
     *          size of the X dimension, in world coordinates (meters)
     * @param theYDimension_m
     *          size of the Y dimension, in world coordinates (meters)
     */
    WorldBox(float theXDimension_m, float theYDimension_m);

    /**
     * Copy constructor.
     */
    WorldBox(const WorldBox&) = delete;

    /**
     * Move constructor.
     */
    WorldBox(WorldBox&&) = delete;

    /**
     * Destructor.
     */
    ~WorldBox();

    /**
     * Draws the world box.
     *
     * @param theViewMatrix
     *          view matrix
     * @param theProjectionMatrix
     *          projection matrix
     */ 
    void draw(glm::mat4 &theViewMatrix,
              glm::mat4 &theProjectionMatrix);

    protected:

    private:

    /**
     * Creates the shader program for the world box.
     */
    void createShader();

    /** Model matrix. */
    glm::mat4 myModelMatrix;

    /** Shader for the box. */
    ShaderProgram myShaderProgram;

    /** Shaders to make up the program. */
    std::vector<std::shared_ptr<Shader>> myShaders;

    /** Vertex array object for the box. */
    GLuint myVAO;

    /** Vertex buffer object for the box. */
    GLuint myVBO;

    /** Size of the X dimension, meters. */
    const float myXDimension_m;

    /** Size of Y dimension, meters. */
    const float myYDimension_m;
  };
}
