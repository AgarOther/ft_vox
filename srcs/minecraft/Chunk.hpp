#pragma once

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "fastnoiselite/FastNoiseLite.h"
#include "Location.hpp"

#define CHUNK_HEIGHT 256
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define VERTICES_COUNT 8

class World;

class Chunk
{
	public:
		Chunk(int chunkX, int chunkZ, const FastNoiseLite & noise):
			_chunkX(chunkX), _chunkZ(chunkZ), _noise(noise), _vao(0), _vbo(0), _ibo(0), _generated(false) {}
		~Chunk();

		void					generateBlocks();
		void					generateMesh(const TextureAtlas & atlas, World * world);
		void					uploadMesh();
		void					render(const Shader & shader) const;
		BlockType				getBlockAt(const Location & loc);
		BlockType				getBlockAtChunkLocation(const Location & loc);
		void					changeBlockAt(const Location & loc, Material newMaterial);

		int						getChunkX() const { return _chunkX; }
		int						getChunkZ() const { return _chunkZ; }
	private:
		int						_chunkX;
		int						_chunkZ;
		int						_indicesCount;
		FastNoiseLite			_noise;
		GLuint					_vao;
		GLuint					_vbo;
		GLuint					_ibo;
		GLuint					_tbo;
		uint8_t					_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
		TextureAtlas			_atlas;
		std::vector<float>		_vertices;
		std::vector<uint32_t>	_indices;
		bool					_generated;
		World *					_world;

		bool					isBlockVisible(int x, int y, int z);
		bool					isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right);
		float					getBrightness(BlockFace face, int x, int y, int z);
};	
