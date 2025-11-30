#version 330 core

out vec4		color;

uniform float	opacity;

void main()
{
    color = vec4(1.0f, 1.0f, 1.0f, opacity);
}
