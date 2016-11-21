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
#include "ActorUpdateCallback.h"
#include "ShaderProgram.h"

namespace QS
{
  class Actor;
  class Shader;

  /**
   * This class draws all of the Actors in the world. Actor position and color
   * data is stored via the ActorUpdateCallback then drawn via the draw
   * function.
   */
  class Actors : public ActorUpdateCallback
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
     * Callback for Actor updates.
     *
     * @param theActor
     *          updated Actor
     */
    virtual void actorUpdate(const Actor *theActor) noexcept override;

    /**
     * Draws all of the provided Actors.
     *
     * @param theViewMatrix
     *          view matrix
     * @param theProjectionMatrix
     *          projection matrix
     */
    void draw(glm::mat4 &theViewMatrix,
              glm::mat4 &theProjectionMatrix);
    /**
     * Copy assignment operator.
     */
    Actors& operator=(const Actors&) = delete;

    /**
     * Move assignment operator.
     */
    Actors& operator=(Actors&&) = delete;

    /**
     * Resets color and model matrices in anticipation of another set of Actor
     * updates.
     */
    void resetColorsAndModels() noexcept;

    protected:

    private:

    /**
     * Points in the circle. Center point, 361 circumference points (extra
     * point is a duplicate of the first circumference point, but it closes the
     * circle).
     */
    static constexpr auto NUM_CIRCLE_VERTICES = 362;

    /** Number of vertices for orientation indicator shape. */
    static constexpr auto NUM_ORIENTATION_IND_VERTICES = 3;

    /**
     * Creates the shader program for the world box.
     */
    void createShader();

    /** Buffer to hold circle verticies. */
    GLuint myCircleVertexBuffer;

    /** Actor color vectors. Parallel to myModelMatrices. */
    std::vector<glm::vec3> myColorVectors;

    /** Actor model matrices. Parallel to myColorVectors. */
    std::vector<glm::mat4> myModelMatrices;

    /** Shader for the Actor. */
    ShaderProgram myShaderProgram;

    /** Shaders which comprise the program. */
    std::vector<std::shared_ptr<Shader>> myShaders;

    /** Vertex attributes object. */
    GLuint myVAO;
  };
}
