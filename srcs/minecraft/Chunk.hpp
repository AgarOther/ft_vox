#pragma once

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "fastnoiselite/FastNoiseLite.h"
#include "Location.hpp"

#define CHUNK_HEIGHT 256
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16

class World;

class Chunk
{
	public:
		Chunk(int chunkX, int chunkZ, const FastNoiseLite & noise);
		~Chunk();

		void			generateMesh(const TextureAtlas & atlas, World * world);
		void			render(const Shader & shader) const;
		BlockType		getBlockAt(const Location & loc);
		BlockType		getBlockAtChunkLocation(const Location & loc);

		int				getChunkX() const { return _chunkX; }
		int				getChunkZ() const { return _chunkZ; }
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
		bool			isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right);
		float			getBrightness(BlockFace face, int x, int y, int z);
};
