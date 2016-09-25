#pragma once

/**
 * @file ShaderProgram.h
 * @brief Encapsulates a single OpenGL shader program
 *
 * @author Michael Albers
 */

#include <GL/glew.h>
#include "GLFW/glfw3.h"

namespace QS
{
  class Shader;

  class ShaderProgram
  {
    public:

    /**
     * Default constructor. Creates the program, specific shaders need to be
     * attached later.
     */
    ShaderProgram() noexcept;

    /**
     * Copy constructor.
     */
    ShaderProgram(const ShaderProgram&) = delete;

    /**
     * Move constructor.
     */
    ShaderProgram(ShaderProgram&&) = delete;

    /**
     * Destructor. Deletes the shader.
     */
    ~ShaderProgram();

    /**
     * Attaches the given shader to this program. User must call 'link' before
     * this shader will have any affect on the program. This isn't done
     * automatically to avoid multiple unnecessarily links when adding multiple
     * shaders.
     *
     * @param theShader
     *          shader to add
     */
    void attachShader(Shader &theShader) noexcept;

    /**
     * Detaches the given shader from this program
     *
     * @param theShader
     *          shader to detach
     */
    void detachShader(Shader &theShader) noexcept;

    /**
     * Returns the shader program.
     *
     * @return the shader program
     */
    GLuint getProgram() const noexcept;

    /**
     * Links all of the attached shaders into this program.
     *
     * @throw std::logic_error
     *         on link failure
     */
    void link();

    /**
     * Copy assignment operator.
     */
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    /**
     * Move assignment operator.
     */
    ShaderProgram& operator=(ShaderProgram&&) = delete;

    /**
     * GLuint conversion operator. Returns shader program.
     */
    operator GLuint() const noexcept;

    /**
     * Use this shader program.
     */
    void use() const noexcept;

    protected:

    private:

    /** Shader program */
    GLuint myProgram;
  };
}
