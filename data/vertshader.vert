#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 textureCoordIn;

smooth out vec4 fColor;
out vec2 textureCoord;

uniform mat4 perspectiveMatrix;
uniform mat4 transformMatrix;
uniform mat4 cameraTransform;

void main()
{
	vec4 cameraPos = transformMatrix * position;
	vec4 cameraPerspective = cameraTransform * cameraPos;
	gl_Position = perspectiveMatrix * cameraPerspective;

	fColor = inColor;
	textureCoord = textureCoordIn;
}
