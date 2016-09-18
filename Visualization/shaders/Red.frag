#version 430 core

/**
 * @file Red.frag
 * @brief Colors the fragment red.
 *
 * @author Michael Albers
 */

layout (location = 0) out vec4 color;

void main(void)
{
  color = vec4(1.0, 0.0, 0.0, 0.0);
}
