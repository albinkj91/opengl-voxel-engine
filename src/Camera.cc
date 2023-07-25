#define GL_GLEXT_PROTOTYPES
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

Camera::Camera(glm::vec4 const& position,
		glm::vec4 const& direction,
		glm::vec4 const& up)
	:position{position}, direction{direction}, up{up}
{ }

void Camera::set_camera_matrix(unsigned int program) const
{
	glm::mat4 matrix{
		glm::lookAt(
			glm::vec3{position},
			glm::vec3{position} + glm::vec3{direction},
			glm::vec3{up})};

	glUseProgram(program);
	int camera_transform_location{glGetUniformLocation(program, "cameraTransform")};

	glUniformMatrix4fv(camera_transform_location, 1, GL_FALSE, glm::value_ptr(matrix));
	glUseProgram(0);
}

void Camera::translate_x(float const offset)
{
	position += glm::vec4{offset * glm::normalize(glm::cross(glm::vec3{up}, glm::vec3{direction})), 1.f};
}

void Camera::translate_y(float const offset)
{
	position += glm::vec4{0.f, offset, 0.f, 0.f};
}

void Camera::translate_z(float const offset)
{
	position += offset * direction;
}

void Camera::rotate_x(float const angle)
{
	glm::mat4 matrix{1.0f};
	matrix = glm::rotate(matrix, angle, glm::vec3{up});
	direction = matrix * direction;
}

void Camera::rotate_y(float const angle)
{
	glm::mat4 matrix{1.0f};

	matrix = glm::rotate(matrix,
		angle,
		glm::normalize(glm::cross(glm::vec3{up}, glm::vec3{direction})));

	direction = matrix * direction;
}
