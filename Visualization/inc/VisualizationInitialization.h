#pragma once

/**
 * @file VisualizationInitialization.h
 * @brief VisualizationInitialization class
 *
 * @author Michael Albers
 */

namespace QS
{
  class VisualizationInitialization
  {
    public:

    /**
     * Initializes GLEW. Must be called after OpenGL context has been created
     * or initialization will fail.
     *
     * @throw std::logic_error
     *         on failure
     */
    static void InitializeGLEW();

    /**
     * Initializes GLFW and sets the OpenGL version. Must be called before any
     * GLFW/OpenGL/etc. calls.
     *
     * @throws std::logic_error
     *          on init failure
     */
    static void InitializeGLFW();

    /**
     * Terminates GLEW.
     */
    static void TerminateGLEW() noexcept;

    /**
     * Terminates GLFW.
     */
    static void TerminateGLFW() noexcept;

    protected:

    private:
  };
}
