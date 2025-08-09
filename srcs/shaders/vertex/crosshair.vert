#version 330 core

layout (location = 0) in vec2 aPos;

uniform float aspectRatio;

void main()
{
	float scale = 0.4f;
	gl_Position = vec4((aPos.x * scale) / aspectRatio, aPos.y * scale, 0, 1.0);
}
