#version 330 core

out vec4 FragColor;

in vec2 texCoord;
flat in int texID;
flat in int blockFace;

uniform sampler2D textures[6];
uniform int texCount;
uniform int tintFace;
uniform vec4 tintCol;

void main()
{
	if (texID >= 0 && texID < texCount)
	{
		if (tintFace > 0 && tintFace == blockFace)
			FragColor = texture(textures[texID], texCoord) * tintCol * 1;
		else
    		FragColor = texture(textures[texID], texCoord);
	}
    else
        FragColor = vec4(1.0f);
}
