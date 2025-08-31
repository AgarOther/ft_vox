#version 330 core

in vec3 texCoord;
out vec4 color;

uniform samplerCube skybox;

void main()
{
	color = vec4(102.0f / 255.0f, 154.0f / 255.0f, 1.0f, 1.0f);
	// color = texture(skybox, texCoord);
}
