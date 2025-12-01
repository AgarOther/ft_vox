#pragma once

#include "Shader.hpp"

class Filter
{
	public:
		Filter();
		~Filter();

		void			draw(const glm::vec4 & filterColor);
	private:
		GLuint			_vao;
		GLuint			_vbo;
		GLuint			_ibo;
		Shader			_shader;
		unsigned int	_indicesSize;
};
