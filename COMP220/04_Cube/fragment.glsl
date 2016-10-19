#version 330 core

in vec3 colour;
in vec2 uv;
uniform sampler2D textureSampler;

out vec4 fragmentColour;

void main()
{
	fragmentColour = vec4(colour, 1.0);
	//fragmentColour = texture(textureSampler, uv);
}
