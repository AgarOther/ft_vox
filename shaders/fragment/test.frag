#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
flat in int texID;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	FragColor = (texID == 0) ? texture(tex0, texCoord) :
            	(texID == 1) ? texture(tex1, texCoord) :
            	texture(tex2, texCoord);

}
