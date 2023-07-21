#version 450

out vec4 fragColor;

in vec2 textureCoord;
uniform sampler2D textureSampl;

void main()
{
	fragColor = texture(textureSampl, textureCoord);
}
