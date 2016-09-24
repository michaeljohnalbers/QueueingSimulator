#version 430 core

/**
 * @file WorldBox.frag
 * @brief Fragment shader for the WorldBox class
 *
 * @author Michael Albers
 */

in vec4 fragmentColor;

out vec4 color;

void main()
{
  color = fragmentColor;
}
