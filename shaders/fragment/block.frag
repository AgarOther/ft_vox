#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
flat in int texID;

uniform sampler2D textures[6];
uniform int texCount;

void main()
{
	if (texID >= 0 && texID < texCount)
        FragColor = texture(textures[texID], texCoord);
    else
        FragColor = vec4(1.0f);
}
