#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "Player.hpp"

class BlockOverlay
{
	public:
		BlockOverlay();
		~BlockOverlay();

		void			draw(Player * player, float deltaTime);
	private:
		GLuint			_vao;
		GLuint			_vbo;
		GLuint			_ibo;
		unsigned int	_indicesSize;
		Shader			_shader;
};
