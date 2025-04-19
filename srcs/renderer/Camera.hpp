/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 02:27:04 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 15:09:07 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP
# define GLM_ENABLE_EXPERIMENTAL

# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>
# include <glm/gtx/rotate_vector.hpp>
# include <glm/gtx/vector_angle.hpp>
# include "Shader.hpp"


class Camera
{
	public:
		Camera(int width, int height, glm::vec3 position);
		~Camera();

		void setupMatrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform);
		void interceptInputs(GLFWwindow *window);

		// Getters
		glm::vec3 getPosition() const;
		glm::vec3 getOrientation() const;
		glm::vec3 getUp() const;
		int getWidth() const;
		int getHeight() const;
		float getSpeed() const;
		float getSensitivity() const;
		bool hasClicked() const;

		// Setters
		void setPosition(const glm::vec3& position);
		void setOrientation(const glm::vec3& orientation);
		void setUp(const glm::vec3& up);
		void setWidth(int width);
		void setHeight(int height);
		void setSpeed(float s);
		void setSensitivity(float s);
		void setClicked(bool clicked);
	private:
		glm::vec3 _position;
		glm::vec3 _orientation;
		glm::vec3 _altitude;
		int _width;
		int _height;
		float _speed;
		float _sensitivity;
		bool _firstClick;
};

#endif