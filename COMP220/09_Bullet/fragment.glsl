#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;
uniform sampler2D textureSampler;

uniform vec3 lightDirection;

out vec4 fragmentColour;

void main()
{
	vec3 lightDirectionNorm = normalize(lightDirection);
	float diffuseIntensity = dot(normal, lightDirectionNorm);
	fragmentColour = vec4(diffuseIntensity * colour, 1.0) * texture(textureSampler, uv);
}
