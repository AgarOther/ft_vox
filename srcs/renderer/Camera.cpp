#include "Camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
	_altitude = glm::vec3(0.0f, 1.0f, 0.0f);
	_width = width;
	_height = height;
	_position = position;
	_speed = 7.00f;
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

Camera::~Camera()
{
	
}

void Camera::setupMatrix(const Shader & shader)
{
	_view = glm::lookAt(_position, _position + _orientation, _altitude);
	_proj = glm::perspective(glm::radians(_FOV), static_cast<float>(_width) / static_cast<float>(_height), 0.01f, _farPlane);
	shader.setMat4("camMatrix", _proj * _view);
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
bool Camera::hasGuiOn() const { return (_guiOn); }
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
void Camera::setClicked(bool clicked) { _firstClick = clicked; }
void Camera::setGui(bool guiOn) { _guiOn = guiOn; }
void Camera::setLocked(bool lock) { _locked = lock; }
void Camera::setFullscreen(bool fullscreen) { _fullScreen = fullscreen; }
