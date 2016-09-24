#version 430 core

/**
 * @file Actor.vert
 * @brief Vertex shader for drawing Actor images
 *
 * @author: Michael Albers
 */

in vec3 inPosition;
in mat4 inModelMatrix;

out vec4 fragmentColor;

uniform mat4 inViewMatrix;
uniform mat4 inProjectionMatrix;

void main()
{
  gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix *
    vec4(inPosition, 1.0);

  // TODO: change to use input
  fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
}
