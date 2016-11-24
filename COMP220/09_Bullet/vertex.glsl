#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColour;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;

out vec3 colour;
out vec3 normal;
out vec2 uv;

void main()
{
	gl_Position = vpMatrix * modelMatrix * vec4(vertexPos, 1.0);
	colour = vertexColour;
	normal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
	uv = vertexUV;
	uv.y = 1.0 - uv.y;
}
