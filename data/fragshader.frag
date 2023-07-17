#version 450

out vec4 color;

in vec2 textureCoord;
uniform sampler2D textureSampl;

void main()
{
	color = texture(textureSampl, textureCoord);
}
