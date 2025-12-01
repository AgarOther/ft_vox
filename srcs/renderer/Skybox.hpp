#pragma once

#include "Shader.hpp"
#include "Camera.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Skybox
{
	public:
		Skybox();
		~Skybox();

		void	render(const Camera * camera, const Environment environment);
	private:
		GLuint	_vao;
		GLuint	_vbo;
		Shader	_shader;
};
