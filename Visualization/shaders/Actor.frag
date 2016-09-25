#version 430 core

/**
 * @file Actor.frag
 * @brief Fragment shader for the Actors class
 *
 * @author Michael Albers
 */

in vec4 fragmentColor;

out vec4 color;

void main()
{
  color = fragmentColor;
}
