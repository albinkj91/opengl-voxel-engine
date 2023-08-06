#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordIn;
layout(location = 2) in vec3 normal;

out vec2 textureCoord;
out float light;

uniform mat4 perspectiveMatrix;
uniform mat4 transformMatrix;
uniform mat4 cameraTransform;

void main()
{
	gl_Position = perspectiveMatrix * cameraTransform * transformMatrix * position;
	textureCoord = textureCoordIn;
	vec3 lightPos = vec3(-200.f, 200.f, -50.f);
	vec3 lightDir = normalize(lightPos - vec3(transformMatrix * position));
	light = max(dot(normal, lightDir), 0.0);
}
