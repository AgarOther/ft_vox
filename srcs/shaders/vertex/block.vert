#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in uint aBlockID;
layout (location = 4) in uint aFaceID;

out vec2 texCoord;
out vec3 normal;
flat out uint blockID;
flat out uint faceID;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1);
	texCoord = aTex;
	normal = mat3(transpose(inverse(model))) * aNormal;
	blockID = aBlockID;
	faceID = aFaceID;
}
