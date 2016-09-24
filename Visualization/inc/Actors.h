#pragma once

/**
 * @file Actors.h
 * @brief Draws all the actors in the world.
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
  class Actor;
  class Shader;

  /**
   * This class draws all of the Actors in the world.
   */
  class Actors
  {
    public:

    /**
     * Default constructor
     */
    Actors();

    /**
     * Copy constructor.
     */
    Actors(const Actors&) = delete;

    /**
     * Move constructor.
     */
    Actors(Actors&&) = delete;

    /**
     * Destructor.
     */
    ~Actors();

    /**
     * Draws all of the provided Actors.
     *
     * @param theViewMatrix
     *          view matrix
     * @param theProjectionMatrix
     *          projection matrix
     * @param theActors
     *          actors to draw
     */ 
    void draw(glm::mat4 &theViewMatrix,
              glm::mat4 &theProjectionMatrix,
              const std::vector<Actor*> &theActors);
    /**
     * Copy assignment operator.
     */
    Actors& operator=(const Actors&) = delete;

    /**
     * Move assignment operator.
     */
    Actors& operator=(Actors&&) = delete;

    protected:

    private:

    /**
     * Points in the circle. Center point, 361 circumference points (extra
     * point is a duplicate of the first circumference point, but it closes the
     * circle).
     */
    static constexpr auto NUM_CIRCLE_VERTICES = 362;

    /**
     * Creates the shader program for the world box.
     */
    void createShader();

    /** Buffer to hold circle verticies. */
    GLuint myCircleVertexBuffer;

    /** Shader for the Actor. */
    ShaderProgram myShaderProgram;

    /** Shaders which comprise the program. */
    std::vector<std::shared_ptr<Shader>> myShaders;

    /** Vertex attributes object. */
    GLuint myVAO;
  };
}
