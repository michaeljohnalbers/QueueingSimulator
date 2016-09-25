#version 430 core

/**
 * @file WorldBox.vert
 * @brief Vertex shader for the WorldBox class
 *
 * @author Michael Albers
 */

in vec3 inPosition;
in vec4 inColor;

out vec4 fragmentColor;

uniform mat4 ModelViewProjection;

void main()
{
  gl_Position = ModelViewProjection * vec4(inPosition, 1.0);
  fragmentColor = inColor;
}
