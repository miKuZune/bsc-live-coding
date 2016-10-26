#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColour;
layout(location = 2) in vec2 vertexUV;

uniform mat4 mvp;

out vec3 colour;
out vec2 uv;

void main()
{
	gl_Position = mvp * vec4(vertexPos, 1.0);
	colour = vertexColour;
	uv = vertexUV;
	uv.y = 1.0 - uv.y;
}
