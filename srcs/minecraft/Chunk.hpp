#pragma once

#include "Shader.hpp"

#define CHUNK_HEIGHT 256
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16

class Chunk
{
	public:
		Chunk(int chunkX, int chunkZ);
		~Chunk();

		void	generateMesh();
		void	render(const Shader & shader) const;
	private:
		int		_chunkX;
		int		_chunkZ;
		int		_indicesCount;
		GLuint	_vao;
		GLuint	_vbo;
		GLuint	_ibo;
		uint8_t	_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
};
