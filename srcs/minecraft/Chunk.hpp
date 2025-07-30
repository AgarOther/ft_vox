#pragma once

#include "Shader.hpp"
#include "TextureAtlas.hpp"

#define CHUNK_HEIGHT 256
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16

class Chunk
{
	public:
		Chunk(int chunkX, int chunkZ);
		~Chunk();

		void			generateMesh(const TextureAtlas & atlas);
		void			render(const Shader & shader) const;
	private:
		int				_chunkX;
		int				_chunkZ;
		int				_indicesCount;
		GLuint			_vao;
		GLuint			_vbo;
		GLuint			_ibo;
		GLuint			_tbo;
		uint8_t			_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
		TextureAtlas	_atlas;

		bool			isBlockVisible(int x, int y, int z);
		bool			isFaceVisible(BlockFace face, int x, int y, int z);
};
