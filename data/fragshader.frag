#version 450

out vec4 fragColor;

in vec2 textureCoord;
uniform sampler2D textureSampl;

void main()
{
	vec4 texColor = texture(textureSampl, textureCoord);
	if(texColor.a < 0.95f)
		discard;
	fragColor = texture(textureSampl, textureCoord);
}
