#include "Camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
	_altitude = glm::vec3(0.0f, 1.0f, 0.0f);
	_width = width;
	_height = height;
	_position = position;
	_speed = 6.50f;
	_baseSpeed = _speed;
	_sensitivity = 200.0f;
	_firstClick = true;
	_farPlane = 500.0f;
	_FOV = 90.0f;
	_pitch = 0.0f;
	_yaw = 90.0f;
	_fullScreen = false;
	_guiOn = false;
	_locked = false;

	glm::vec3 direction;
	direction.x = cosf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	direction.y = sinf(glm::radians(_pitch));
	direction.z = sinf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	_orientation = glm::normalize(direction);
}

void Camera::setupMatrix(const Shader & shader)
{
	shader.bind();
	_view = glm::lookAt(_position, _position + _orientation, _altitude);
	_proj = glm::perspective(glm::radians(_FOV), static_cast<float>(_width) / static_cast<float>(_height), 0.01f, _farPlane);
	shader.setMat4("camMatrix", _proj * _view);
}
