#version 330 core

uniform sampler2D textureAtlas;
uniform vec3 lightDir;

flat in uint blockID;
flat in uint faceID;
in vec2 texCoord;
in vec3 normal;

out vec4 color;

void main()
{
	vec4 texColor = texture(textureAtlas, texCoord);

	// Tint for the grass block, blockID and faceID really need better management...
	vec3 tint = vec3(1.0f);
	if (blockID == 8u || (blockID == 1u && faceID == 4u))
		tint = vec3(0.55, 0.80, 0.35);

	// Calculate lighting
	vec3 norm = normalize(normal);
	vec3 light = normalize(-lightDir);
	float diff = max(dot(norm, light), 0.2);

	color = vec4(texColor.rgb * diff * tint, texColor.a);
}
