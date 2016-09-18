#pragma once

/**
 * @file Shader.h
 * @brief Encapsulates a single OpenGL shader (of any type)
 *
 * @author Michael Albers
 */

#include <string>
#include <GL/glew.h>
#include "GLFW/glfw3.h"

namespace QS
{
  class Shader
  {
    public:

    /**
     * Default constructor.
     */
    Shader() = delete;

    /**
     * Constructor.
     *
     * @param theSource
     *          shader source
     * @param theType
     *          type of shader (see GL_*_SHADER constants)
     * @throws std::logic_error
     *          on any error creating the shader
     */
    Shader(const std::string &theSource, GLenum theType);

    /**
     * Copy constructor.
     */
    Shader(const Shader&) = delete;

    /**
     * Move constructor.
     */
    Shader(Shader&&) = delete;

    /**
     * Destructor. Deletes the shader.
     */
    ~Shader();

    /**
     * Copy assignment operator.
     */
    Shader& operator=(const Shader&) = delete;

    /**
     * Move assignment operator.
     */
    Shader& operator=(Shader&&) = delete;

    /**
     * GLuint conversion operator. Returns shader.
     */
    operator GLuint() const noexcept;

    /**
     * Returns the shader.
     *
     * @return the shader
     */
    GLuint getShader() const noexcept;

    protected:

    private:

    /**
     * Compiles the shader.
     *
     * @throw std::logic_error
     *         on compilation error
     */
    void compile();

    /**
     * Loads shader source for later compilation.
     *
     * @param theSource
     *          shader source
     */
    void loadSource(const std::string &theSource) noexcept;

    /** Shader ID */
    GLuint myShader = 0;
  };
}
