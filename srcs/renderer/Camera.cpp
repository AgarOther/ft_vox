/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 02:31:27 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 00:59:29 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
	this->_altitude = glm::vec3(0.0f, 1.0f, 0.0f);
	this->_width = width;
	this->_height = height;
	this->_position = position;
	this->_speed = 0.01f;
	this->_baseSpeed = 0.01f;
	this->_sensitivity = 200.0f;
	this->_firstClick = true;
	this->_pitch = 0.0f;
	this->_yaw = 90.0f;
	this->_FOV = 90.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	direction.y = sin(glm::radians(this->_pitch));
	direction.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	this->_orientation = glm::normalize(direction);

	this->_guiOn = false;
}

Camera::~Camera()
{
	
}

void Camera::setupMatrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	view = glm::lookAt(this->_position, this->_position + this->_orientation, this->_altitude);
	proj = glm::perspective(glm::radians(FOVdeg), static_cast<float>(this->_width) / this->_height, nearPlane, farPlane);
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), uniform), 1, GL_FALSE, glm::value_ptr(proj * view));
	this->_FOV = FOVdeg;
}

static glm::vec3 translateDirection(float yaw, float pitch)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	return (direction);
}

void Camera::interceptInputs(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	
	/* GPT Code*/
	// GPT my friend who helps me with the weirdest maths
	// Calculate forward vector for movement based on pitch (only in X-Z plane)
	glm::vec3 forward;
	forward.x = cos(glm::radians(this->_pitch)) * cos(glm::radians(this->_yaw)); // X component based on pitch
	forward.z = cos(glm::radians(this->_pitch)) * sin(glm::radians(this->_yaw)); // Z component based on pitch
	forward.y = 0.0f; // No vertical movement based on pitch, set Y to 0

	// Normalize the forward vector to maintain consistent speed
	forward = glm::normalize(forward);

	// Calculate the right vector (strafe) using yaw, no pitch influence
	glm::vec3 right = glm::normalize(glm::cross(forward, this->_altitude)); // Right direction is based on forward and altitude (up vector)
	/* End GPT Code*/

	// Key management
	if ((glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP)) == GLFW_PRESS)
		this->_position += this->_speed * forward;
	if ((glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) == GLFW_PRESS)
		this->_position += this->_speed * -right;
	if ((glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN)) == GLFW_PRESS)
		this->_position += this->_speed * -forward;
	if ((glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) == GLFW_PRESS)
		this->_position += this->_speed * right;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		this->_position += this->_speed * this->_altitude;
	if (glfwGetKey(window, GLFW_KEY_MENU) == GLFW_PRESS)
		this->_position += this->_speed * -this->_altitude;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		this->_speed = this->_baseSpeed * 1.5f;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		this->_speed = this->_baseSpeed;

	// Prevents toggling every frame
	static bool lastFramePressed = false;
	bool keyPressed = glfwGetKey(window, GLFW_KEY_M);
	if (keyPressed && !lastFramePressed)
		this->_guiOn = !this->_guiOn;

	// Mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
	{
		double mouseX, mouseY;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (this->_firstClick)
		{
			glfwSetCursorPos(window, (this->_width / 2), (this->_height / 2));
			this->_firstClick = false;
		}
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = this->_sensitivity * (float)(mouseY - (this->_height / 2)) / (float)this->_height;
		float rotY = this->_sensitivity * (float)(mouseX - (this->_width  / 2)) / (float)this->_width;

		this->_yaw += rotY;
    	this->_pitch -= rotX;

		if (this->_pitch > 89.99f)
			this->_pitch = 89.99f;
		else if (this->_pitch < -89.99f)
			this->_pitch = -89.99f;
		
		if (this->_yaw < -179.99f)
			this->_yaw = 180.0f;
		else if (this->_yaw > 179.99f)
			this->_yaw = -180.0f;

		this->_orientation = translateDirection(this->_yaw, this->_pitch);
	
		glfwSetCursorPos(window, (this->_width / 2), (this->_height / 2));
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		this->_firstClick = true;
	}
	lastFramePressed = keyPressed;
}

void Camera::teleport(Location location, float yaw, float pitch)
{
	glm::vec3 newPos;

	newPos.x = location.getX();
	newPos.y = location.getY();
	newPos.z = location.getZ();
	this->_position = newPos;
	this->_yaw = yaw;
	this->_pitch = pitch;
	this->_orientation = translateDirection(this->_yaw, this->_pitch);
}

// Getters
glm::vec3 Camera::getPosition() const { return this->_position; }
glm::vec3 Camera::getOrientation() const { return this->_orientation; }
glm::vec3 Camera::getAltitude() const { return this->_altitude; }
int Camera::getWidth() const { return this->_width; }
int Camera::getHeight() const { return this->_height; }
float Camera::getSpeed() const { return this->_speed; }
float Camera::getBaseSpeed() const { return this->_baseSpeed; }
float Camera::getSensitivity() const { return this->_sensitivity; }
float Camera::getYaw() const { return this->_yaw; }
float Camera::getPitch() const { return this->_pitch; }
float Camera::getFOV() const { return this->_FOV; }
bool Camera::hasClicked() const { return (this->_firstClick); }
bool Camera::hasGuiOn() const { return (this->_guiOn); }

// Setters
void Camera::setPosition(const glm::vec3 &position) { this->_position = position; }
void Camera::setOrientation(const glm::vec3 &orientation) { this->_orientation = orientation; }
void Camera::setAltitude(const glm::vec3 &altitude) { this->_altitude = altitude; }
void Camera::setWidth(int width) { this->_width = width; }
void Camera::setHeight(int height) { this->_height = height; }
void Camera::setSpeed(float s) { this->_speed = s; }
void Camera::setBaseSpeed(float s) { this->_baseSpeed = s; }
void Camera::setSensitivity(float s) { this->_sensitivity = s; }
void Camera::setYaw(float s) { this->_yaw = s; }
void Camera::setPitch(float s) { this->_pitch = s; }
void Camera::setFOV(float s) { this->_FOV = s; }
void Camera::setClicked(bool clicked) { this->_firstClick = clicked; }
void Camera::setGuiOn(bool guiOn) { this->_guiOn = guiOn; }
