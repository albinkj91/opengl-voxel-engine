#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 inColor;

smooth out vec4 fColor;
uniform mat4 perspectiveMatrix;
uniform mat4 rotationMatrix;

void main()
{
	fColor = inColor;
	gl_Position = perspectiveMatrix * (rotationMatrix * position);
}
