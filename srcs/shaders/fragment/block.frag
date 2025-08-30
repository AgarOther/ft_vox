#version 330 core

uniform sampler2D textureAtlas;
uniform vec3 lightDir;

in vec3 tint;
in vec2 texCoord;
in vec3 normal;

out vec4 color;

void main()
{
	vec4 texColor = texture(textureAtlas, texCoord);

	// Calculate lighting
	vec3 norm = normalize(normal);
	vec3 light = normalize(-lightDir);
	float diff = max(dot(norm, light), 0.2);

	color = vec4(texColor.rgb * diff * tint, texColor.a);
}
