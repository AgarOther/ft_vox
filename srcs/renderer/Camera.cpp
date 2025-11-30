#include "Camera.hpp"
#include "Chunk.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
	_altitude = glm::vec3(0.0f, 1.0f, 0.0f);
	_width = width;
	_height = height;
	_position = position;
	_speed = PLAYER_SPEED;
	_baseSpeed = _speed;
	_sensitivity = 200.0f;
	_farPlane = 1500.0f;
	_FOV = 80.0f;
	_pitch = 0.0f;
	_yaw = 90.0f;
	_fullScreen = false;
	_guiOn = false;
	_locked = false;
	_renderDistance = 12;
	_fogActive = true;
	_fogStart = FOG_START;
	_fogEnd = FOG_END;
	_fogColors[OVERWORLD] = glm::vec3(FOG_COLOR_OVERWORLD);
	_fogColors[NETHER] = glm::vec3(FOG_COLOR_NETHER);
	_fogColors[THE_END] = glm::vec3(FOG_COLOR_THE_END);

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
	shader.setInt("fogActive", _fogActive);
	shader.setFloat("fogStart", _fogStart * _renderDistance * CHUNK_DEPTH);
	shader.setFloat("fogEnd", _fogEnd * _renderDistance * CHUNK_DEPTH);
	shader.setVec3("fogColor", _fogColors[environment]);
	shader.setVec3("cameraPos", _position);
}

glm::vec3 Camera::computeForward()
{
	glm::vec3 forward;
	forward.x = cosf(glm::radians(_pitch)) * cosf(glm::radians(_yaw)); // X component based on pitch
	forward.z = cosf(glm::radians(_pitch)) * sinf(glm::radians(_yaw)); // Z component based on pitch
	forward.y = 0.0f; // No vertical movement based on pitch, set Y to 0

	// Normalize the forward vector to maintain consistent speed
	forward = glm::normalize(forward);
	return forward;
}
