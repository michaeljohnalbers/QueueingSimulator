#version 430 core

/**
 * @file PassThrough.vert
 * @brief Straight pass through vertex shader
 *
 * Taken from OpenGl Programming Guide, Eigth Edition
 */

layout(location = 0) in vec4 vPosition;

void main()
{
  gl_Position = vPosition;
}
