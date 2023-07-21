#version 450

layout(location = 0) in vec4 position;
//layout(location = 1) in vec2 textureCoordIn;
out vec4 color;

//out vec2 textureCoord;

uniform mat4 perspectiveMatrix;
uniform mat4 transformMatrix;
uniform mat4 cameraTransform;

void main()
{
	vec4 cameraPos = transformMatrix * position;
	vec4 cameraPerspective = cameraTransform * cameraPos;
	gl_Position = perspectiveMatrix * cameraPerspective;

	color = vec4(1.0, 1.0, 1.0, 1.0);
	//textureCoord = textureCoordIn;
}
