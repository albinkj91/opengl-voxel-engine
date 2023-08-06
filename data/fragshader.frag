#version 450

out vec4 fragColor;

in vec2 textureCoord;
in float light;

uniform sampler2D textureSampl;

void main()
{
	vec4 texColor = texture(textureSampl, textureCoord);
	if(texColor.a < 0.95f)
		discard;

	float ambience = 0.2f;
	fragColor = texture(textureSampl, textureCoord) * (light + ambience);
}
