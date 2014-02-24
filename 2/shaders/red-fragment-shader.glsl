#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColour;

// Ouput data
out vec3 color;

void main()
{
	// Output color = color specified in the vertex shader,
	// interpolated between all 3 surrounding vertices
	color = vec3(0.8f, 0.0f, 0.0f);
}
