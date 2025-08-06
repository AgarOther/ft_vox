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

		void	render(const Camera * camera);
	private:
		GLuint	_vao;
		GLuint	_vbo;
		GLuint	_textureID;
		Shader	_shader;
};
