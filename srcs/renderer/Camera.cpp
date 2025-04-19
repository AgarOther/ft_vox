/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 02:31:27 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 17:32:38 by scraeyme         ###   ########.fr       */
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
	this->_pitch = 0.0f;
	this->_yaw = -90.0f;
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
}

void Camera::interceptInputs(GLFWwindow *window)
{
	// Keyboard inputs
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
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

	if ((glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP)) == GLFW_PRESS)
		this->_position += this->_speed * forward;
	if ((glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) == GLFW_PRESS)
		this->_position += this->_speed * -right;
	if ((glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN)) == GLFW_PRESS)
		this->_position += this->_speed * -forward;
	if ((glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) == GLFW_PRESS)
		this->_position += this->_speed * right;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->_position += this->_speed * this->_altitude;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		this->_position -= this->_speed * this->_altitude;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		this->_position += this->_speed * this->_altitude;
	if (glfwGetKey(window, GLFW_KEY_MENU) == GLFW_PRESS)
		this->_position += this->_speed * -this->_altitude;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		this->_speed = 0.025f;
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

		float rotX = this->_sensitivity * (float)(mouseY - (this->_height / 2)) / (float)this->_height;
		float rotY = this->_sensitivity * (float)(mouseX - (this->_width  / 2)) / (float)this->_width;

		this->_yaw += rotY;
    	this->_pitch -= rotX;

		if (this->_pitch > 89.0f)
			this->_pitch = 89.0f;
		else if (this->_pitch < -89.0f)
			this->_pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
		direction.y = sin(glm::radians(this->_pitch));
		direction.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
		this->_orientation = glm::normalize(direction);
	
		glfwSetCursorPos(window, (this->_width / 2), (this->_height / 2));
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		this->_firstClick = true;
	}		
}
