#pragma once

/**
 * @file Exits.h
 * @brief Draws all the exits in the world.
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
  class Exit;
  class Shader;

  /**
   * This class draws all of the Exits in the world.
   */
  class Exits
  {
    public:

    /**
     * Default constructor
     */
    Exits();

    /**
     * Copy constructor.
     */
    Exits(const Exits&) = delete;

    /**
     * Move constructor.
     */
    Exits(Exits&&) = delete;

    /**
     * Destructor.
     */
    ~Exits();

    /**
     * Draws all of the provided Exits.
     *
     * @param theViewMatrix
     *          view matrix
     * @param theProjectionMatrix
     *          projection matrix
     * @param theExits
     *          exits to draw
     */ 
    void draw(glm::mat4 &theViewMatrix,
              glm::mat4 &theProjectionMatrix,
              const std::vector<Exit*> &theExits);
    /**
     * Copy assignment operator.
     */
    Exits& operator=(const Exits&) = delete;

    /**
     * Move assignment operator.
     */
    Exits& operator=(Exits&&) = delete;

    protected:

    private:

    /**
     * Points in the circle. 361 circumference points (extra point is a
     * duplicate of the first circumference point, but it closes the circle).
     */
    static constexpr auto NUM_CIRCLE_VERTICES = 361;

    /** Number of vertices for cross. */
    static constexpr auto NUM_CROSS_VERTICES = 4;

    /**
     * Creates the shader program for the world box.
     */
    void createShader();

    /** Buffer to hold circle verticies. */
    GLuint myCircleVertexBuffer;

    /** Shader for the Exit. */
    ShaderProgram myShaderProgram;

    /** Shaders which comprise the program. */
    std::vector<std::shared_ptr<Shader>> myShaders;

    /** Vertex attributes object. */
    GLuint myVAO;
  };
}
