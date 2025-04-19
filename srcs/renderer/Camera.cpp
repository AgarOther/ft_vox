/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 02:31:27 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 15:09:17 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
	this->_orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	this->_altitude = glm::vec3(0.0f, 1.0f, 0.0f);
	this->_width = width;
	this->_height = height;
	this->_position = position;
	this->_speed = 0.01f;
	this->_sensitivity = 200.0f;
	this->_firstClick = true;
}

Camera::~Camera()
{
	
}

void Camera::setupMatrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	view = glm::lookAt(this->_position, this->_position + this->_orientation, this->_altitude);
	proj = glm::perspective(glm::radians(FOVdeg), (float)(this->_width / this->_height), nearPlane, farPlane);
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), uniform), 1, GL_FALSE, glm::value_ptr(proj * view));
}

void Camera::interceptInputs(GLFWwindow *window)
{
	// Keyboard inputs
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	if ((glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP)) == GLFW_PRESS)
		this->_position += this->_speed * this->_orientation;
	if ((glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) == GLFW_PRESS)
		this->_position += this->_speed * -glm::normalize(glm::cross(this->_orientation, this->_altitude));
	if ((glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN)) == GLFW_PRESS)
		this->_position += this->_speed * -this->_orientation;
	if ((glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) == GLFW_PRESS)
		this->_position += this->_speed * glm::normalize(glm::cross(this->_orientation, this->_altitude));
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		this->_position += this->_speed * this->_altitude;
	if (glfwGetKey(window, GLFW_KEY_MENU) == GLFW_PRESS)
		this->_position += this->_speed * -this->_altitude;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		this->_speed = 0.04f;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		this->_speed = 0.01f;

	// Mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
	{
		double mouseX, mouseY;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (this->_firstClick)
		{
			glfwSetCursorPos(window, (this->_width / 2), (this->_height / 2));
			this->_firstClick = false;
		}
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float rotX = this->_sensitivity * (float)(mouseY - (this->_height / 2)) / this->_height;
		float rotY = this->_sensitivity * (float)(mouseX - (this->_height / 2)) / this->_height;

		glm::vec3 newOrientation = glm::rotate(this->_orientation, glm::radians(-rotX), glm::normalize(glm::cross(this->_orientation, this->_altitude)));
		if (abs(glm::angle(newOrientation, this->_altitude) - glm::radians(90.0f)) <= glm::radians(85.0f))
			this->_orientation = newOrientation;
		
		this->_orientation = glm::rotate(this->_orientation, glm::radians(-rotY), this->_altitude);
	
		glfwSetCursorPos(window, (this->_width / 2), (this->_height / 2));
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		this->_firstClick = true;
	}		
}

// Getters
glm::vec3 Camera::getPosition() const { return _position; }
glm::vec3 Camera::getOrientation() const { return _orientation; }
glm::vec3 Camera::getUp() const { return _altitude; }
int Camera::getWidth() const { return _width; }
int Camera::getHeight() const { return _height; }
float Camera::getSpeed() const { return _speed; }
float Camera::getSensitivity() const { return _sensitivity; }
bool Camera::hasClicked() const { return (this->_firstClick); }

// Setters
void Camera::setPosition(const glm::vec3& position) { _position = position; }
void Camera::setOrientation(const glm::vec3& orientation) { _orientation = orientation; }
void Camera::setUp(const glm::vec3& up) { _altitude = up; }
void Camera::setWidth(int width) { _width = width; }
void Camera::setHeight(int height) { _height = height; }
void Camera::setSpeed(float s) { _speed = s; }
void Camera::setSensitivity(float s) { _sensitivity = s; }
void Camera::setClicked(bool clicked) { _firstClick = clicked; }

