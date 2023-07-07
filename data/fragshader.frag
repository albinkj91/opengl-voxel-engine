#version 450

in vec4 fColor;
out vec4 color;

in vec2 textureCoord;
uniform sampler2D textureSampl;

void main()
{
	color = texture(textureSampl, textureCoord);
}
