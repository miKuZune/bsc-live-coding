#version 330 core

uniform float time;

layout(location = 0) in vec3 vertexPos;

out float thing;

void main()
{
	//gl_Position.xyz = vertexPos;
	//gl_Position.w = 1.0;

	gl_Position = vec4(vertexPos, 1.0);
	gl_Position.xyz *= 0.5 + 0.25 * sin(time * time * 10) / time;

	if (gl_Position.x > 0.5)
		thing = 1;
	else
		thing = 0;
}
