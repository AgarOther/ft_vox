#pragma once

#include "GLEW/glew.h"
#include <GL/gl.h>
#include "Player.hpp"

class BlockOverlay
{
	public:
		BlockOverlay(Camera * camera);
		~BlockOverlay();

		void			draw(const Block & targetedBlock);
	private:
		Camera *		_camera;
		GLuint			_vao;
		GLuint			_vbo;
		GLuint			_ibo;
		unsigned int	_indicesSize;
		Shader			_shader;
};
