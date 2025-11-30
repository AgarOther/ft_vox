#version 330 core

out vec4 FragColor;

uniform float opacity;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, opacity);
}
