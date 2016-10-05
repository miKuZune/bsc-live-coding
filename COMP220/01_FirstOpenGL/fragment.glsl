#version 330 core

uniform vec3 myColour;
uniform float time;

in float thing;

out vec3 color;

void main()
{
	color = myColour;
	color.b = sin(time * 10);
	color.r = thing;
}
