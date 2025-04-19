/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 02:27:04 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 17:20:38 by scraeyme         ###   ########.fr       */
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
	private:
		glm::vec3 _position;
		glm::vec3 _orientation;
		glm::vec3 _altitude;
		int _width;
		int _height;
		float _speed;
		float _sensitivity;
		float _yaw;
		float _pitch;
		bool _firstClick;
};

#endif