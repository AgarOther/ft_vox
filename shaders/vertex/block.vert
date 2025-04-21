#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in int aTexID;
layout (location = 3) in int aBlockFace;

out vec2 texCoord;
flat out int texID;
flat out int blockFace;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1);
	texCoord = aTex;
	texID = aTexID;
	blockFace = aBlockFace;
}
