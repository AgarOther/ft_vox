#include "Camera.hpp"
#include "Chunk.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
	_altitude = glm::vec3(0.0f, 1.0f, 0.0f);
	_width = width;
	_height = height;
	_position = position;
	_speed = 50.f;
	_baseSpeed = _speed;
	_sensitivity = 200.0f;
	_farPlane = 1000.0f;
	_FOV = 80.0f;
	_pitch = 0.0f;
	_yaw = 90.0f;
	_fullScreen = false;
	_guiOn = false;
	_locked = false;
	_renderDistance = 16;
	_fogStart = 0.5f;
	_fogEnd = 1.0f;
	_fogColorOverworld = glm::vec3(180.0f / 255.0f, 210.0f / 255.0f, 1.0f);
	_fogColorNether = glm::vec3(30.0f / 255.0f, 0.0f, 0.0f);
	_fogColorEnd = glm::vec3(30.0f / 255.0f, 0.0f, 40.0f / 255.0f);

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

void Camera::setupFog(const Shader & shader, Environment environment)
{
	shader.setFloat("fogStart", _fogStart * _renderDistance * CHUNK_DEPTH);
	shader.setFloat("fogEnd", _fogEnd * _renderDistance * CHUNK_DEPTH);
	shader.setVec3("fogColor", environment == OVERWORLD ? _fogColorOverworld : environment == NETHER ? _fogColorNether : _fogColorEnd);
	shader.setVec3("cameraPos", _position);
}
