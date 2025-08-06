#version 330 core

in vec3 texCoord;
out vec4 color;

uniform samplerCube skybox;

void main()
{
	color = vec4(0.1f, 0.5f, 0.7f, 1.0f);
	// color = texture(skybox, texCoord);
}
