#version 330 core

out vec3 color;

uniform vec3 shape_colour;

void main()
{
	color = shape_colour;
}
