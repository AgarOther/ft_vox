#include "utils.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
	_altitude = glm::vec3(0.0f, 1.0f, 0.0f);
	_width = width;
	_height = height;
	_position = position;
	_speed = 0.10f;
	_baseSpeed = _speed;
	_sensitivity = 200.0f;
	_firstClick = true;
	_farPlane = 500.0f;
	_FOV = 90.0f;
	_pitch = 0.0f;
	_yaw = 90.0f;
	_fullScreen = false;
	_locked = false;

	glm::vec3 direction;
	direction.x = cosf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	direction.y = sinf(glm::radians(_pitch));
	direction.z = sinf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	_orientation = glm::normalize(direction);
}

Camera::~Camera()
{
	
}

void Camera::setupMatrix(const Shader & shader)
{
	_view = glm::lookAt(_position, _position + _orientation, _altitude);
	_proj = glm::perspective(glm::radians(_FOV), static_cast<float>(_width) / static_cast<float>(_height), 0.01f, _farPlane);
	shader.setMat4("camMatrix", _proj * _view);
}

static glm::vec3 translateDirection(const float yaw, const float pitch)
{
	glm::vec3 direction;
	direction.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	direction.y = sinf(glm::radians(pitch));
	direction.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	return (direction);
}

void Camera::interceptInputs(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	
	/* GPT Code */
	// GPT my friend who helps me with the weirdest maths
	// Calculate forward vector for movement based on pitch (only in X-Z plane)
	glm::vec3 forward;
	forward.x = cosf(glm::radians(_pitch)) * cosf(glm::radians(_yaw)); // X component based on pitch
	forward.z = cosf(glm::radians(_pitch)) * sinf(glm::radians(_yaw)); // Z component based on pitch
	forward.y = 0.0f; // No vertical movement based on pitch, set Y to 0

	// Normalize the forward vector to maintain consistent speed
	forward = glm::normalize(forward);

	// Calculate the right vector (strafe) using yaw, no pitch influence
	const glm::vec3 right = glm::normalize(glm::cross(forward, _altitude)); // Right direction is based on forward and altitude (up vector)
	/* End GPT Code */

	// Key management
	if (!_locked)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			_position += _speed * forward;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			_position += _speed * -right;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			_position += _speed * -forward;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			_position += _speed * right;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			_position += _speed * _altitude;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_MENU) == GLFW_PRESS)
			_position += _speed * -_altitude;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			_speed = _baseSpeed * 2.5f;
		else
			_speed = _baseSpeed;
	}

	static bool lastFramePressedF11 = false;
	const bool keyPressedF11 = glfwGetKey(window, GLFW_KEY_F11);
	if (keyPressedF11 && !lastFramePressedF11)
		toggleFullscreen(window, *this);

	// Mouse inputs
	if (!_locked && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
	{
		double mouseX, mouseY;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (_firstClick)
		{
			glfwSetCursorPos(window, (static_cast<float>(_width) / 2), (static_cast<float>(_height) / 2));
			_firstClick = false;
		}
		glfwGetCursorPos(window, &mouseX, &mouseY);

		const float rotX = _sensitivity * static_cast<float>(mouseY - (static_cast<float>(_height) / 2)) / static_cast<float>(_height);
		const float rotY = _sensitivity * static_cast<float>(mouseX - (static_cast<float>(_width) / 2)) / static_cast<float>(_width);

		_yaw += rotY;
    	_pitch -= rotX;

		if (_pitch > 89.99f)
			_pitch = 89.99f;
		else if (_pitch < -89.99f)
			_pitch = -89.99f;
		
		if (_yaw < -179.99f)
			_yaw = 180.0f;
		else if (_yaw > 179.99f)
			_yaw = -180.0f;

		_orientation = translateDirection(_yaw, _pitch);
	
		glfwSetCursorPos(window, (static_cast<float>(_width) / 2), (static_cast<float>(_height) / 2));
	}
	else
	{
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPos(window, (static_cast<float>(_width) / 2), (static_cast<float>(_height) / 2));
		}
		_firstClick = true;
	}
	lastFramePressedF11 = keyPressedF11;
}

// Getters
glm::mat4 Camera::getViewMatrix() const { return _view; }
glm::mat4 Camera::getProjectionMatrix() const { return _proj; }
glm::vec3 Camera::getPosition() const { return _position; }
glm::vec3 Camera::getOrientation() const { return _orientation; }
glm::vec3 Camera::getAltitude() const { return _altitude; }
int Camera::getWidth() const { return _width; }
int Camera::getHeight() const { return _height; }
float Camera::getSpeed() const { return _speed; }
float Camera::getBaseSpeed() const { return _baseSpeed; }
float Camera::getSensitivity() const { return _sensitivity; }
float Camera::getYaw() const { return _yaw; }
float Camera::getPitch() const { return _pitch; }
float Camera::getFOV() const { return _FOV; }
float Camera::getFarPlane() const { return _farPlane; }
bool Camera::hasClicked() const { return (_firstClick); }
bool Camera::isLocked() const { return (_locked); }
bool Camera::isFullscreen() const { return (_fullScreen); }

// Setters
void Camera::setPosition(const glm::vec3 &position) { _position = position; }
void Camera::setOrientation(const glm::vec3 &orientation) { _orientation = orientation; }
void Camera::setAltitude(const glm::vec3 &altitude) { _altitude = altitude; }
void Camera::setWidth(const int width) { _width = width; }
void Camera::setHeight(const int height) { _height = height; }
void Camera::setSpeed(const float s) { _speed = s; }
void Camera::setBaseSpeed(const float s) { _baseSpeed = s; }
void Camera::setSensitivity(const float s) { _sensitivity = s; }
void Camera::setYaw(const float s) { _yaw = s; }
void Camera::setPitch(const float s) { _pitch = s; }
void Camera::setFOV(const float s) { _FOV = s; }
void Camera::setFarPlane(const float farPlane) { _farPlane = farPlane; }
void Camera::setClicked(const bool clicked) { _firstClick = clicked; }
void Camera::setLocked(const bool lock) { _locked = lock; }
void Camera::setFullscreen(const bool fullscreen) { _fullScreen = fullscreen; }
