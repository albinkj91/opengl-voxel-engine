#version 450

in vec4 color;
out vec4 fragColor;

//in vec2 textureCoord;
//uniform sampler2D textureSampl;

void main()
{
	//color = texture(textureSampl, textureCoord);
	fragColor = color;
}
