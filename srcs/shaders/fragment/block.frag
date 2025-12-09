#version 330 core
in vec3				tint;
in vec2				texCoord;
in vec3				normal;
in vec4				worldPos;

out vec4			color;

uniform sampler2D	textureAtlas;
uniform vec3		lightDir;
uniform bool		fogActive;
uniform float		fogStart;
uniform float		fogEnd;
uniform vec3		fogColor;
uniform vec3		cameraPos;

void main()
{
	vec4 texColor = texture(textureAtlas, texCoord);

	if (texColor.a == 0.0)
		discard;

	// Calculate lighting
	vec3 norm = normalize(normal);
	vec3 light = normalize(-lightDir);
	float diff = max(dot(norm, light), 0.25);
	vec4 lightColor = vec4(texColor.rgb * diff, texColor.a);

	float fogFactor = 1;
	// Calculate fog
	if (fogActive)
	{
		float dist = length(worldPos.xyz - cameraPos);
		fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0f, 1.0f);
	}

	color = vec4(mix(fogColor, lightColor.rgb, fogFactor), lightColor.a);
}
