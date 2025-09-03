#version 330 core

in vec3		texCoord;
out vec4	color;

uniform int	environment;

void main()
{
	// Normalize to [0, 1]
	if (environment == 1)
		color = vec4(30.f / 255.f, 0.0f, 0.0f, 1.0f);
	else if (environment == 2)
	{
		float star = (texCoord.x + texCoord.y + texCoord.z) * 5;
		color = vec4((star + 15.0f) / 255.0f, 0.0f, (25.0f + star) / 255.0f, 1.0f);
	}
	else
	{
		float normalized = normalize(texCoord).y;
		normalized = clamp((normalized + 1.0) * 0.5, 0.0, 1.0);

		vec3 skyColor = vec3(102.0/255.0, 154.0/255.0, 1.0);
		vec3 gradientColor = vec3(1.0, 1.0, 1.0);
		color = vec4(mix(gradientColor, skyColor, normalized), 1.0);
	}
}
