#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera(glm::vec4 const& position,
		glm::vec4 const& direction,
		glm::vec4 const& up);

	void set_camera_matrix(unsigned int program) const;
	void translate_x(float const offset);
	void translate_z(float const offset);
	void rotate_x(float const angle);
	void rotate_y(float const angle);

private:
	glm::vec4 position;
	glm::vec4 direction;
	glm::vec4 up;
};

#endif
