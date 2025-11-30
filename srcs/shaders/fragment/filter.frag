#version 330 core

out vec4		color;

uniform vec4	filterColor;

void main()
{
	color = filterColor;
}
