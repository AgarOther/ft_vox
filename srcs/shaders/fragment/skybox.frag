#version 330 core

in vec3 texCoord;
out vec4 color;

void main()
{
	// Normalize to [0, 1]
	float normalized = normalize(texCoord).y;
	normalized = clamp((normalized + 1.0) * 0.5, 0.0, 1.0);

	vec3 skyColor = vec3(102.0/255.0, 154.0/255.0, 1.0);
	vec3 gradientColor = vec3(1.0, 1.0, 1.0);
    color = vec4(mix(gradientColor, skyColor, normalized), 1.0);
}
