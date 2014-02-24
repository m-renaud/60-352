#version 330 core

in vec2 UV;

out vec3 colour;

uniform sampler2D texture_sampler;

void main()
{
	colour = texture2D(texture_sampler, UV).rgb;
}
