#pragma once

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "fastnoiselite/FastNoiseLite.h"
#include "Location.hpp"
#include <cstdint>
#include <mutex>

#define CHUNK_HEIGHT 256
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define VERTICES_COUNT 8

class World;

class Chunk
{
	public:
		Chunk(int chunkX, int chunkZ, World * world):
			_chunkX(chunkX), _chunkZ(chunkZ), _vao(0), _vbo(0), _ibo(0), _bbo(0), _fbo(0),
			 _world(world), _state(IDLE) {}
		~Chunk();

		void					generateBlocks();
		void					generateMesh();
		void					uploadMesh();
		void					unloadMesh();
		void					render(const Shader & shader) const;
		BlockType				getBlockAt(const Location & loc);
		BlockType				getBlockAtChunkLocation(const Location & loc);
		void					changeBlockAt(const Location & loc, Material newMaterial);

		int						getChunkX() const { return _chunkX; }
		int						getChunkZ() const { return _chunkZ; }
		ChunkState				getState() { const std::lock_guard<std::mutex> lg(_stateMutex); return _state; }

		void					setState(ChunkState state) { const std::lock_guard<std::mutex> lg(_stateMutex); _state = state; }
	private:
		int						_chunkX;
		int						_chunkZ;
		GLuint					_vao;
		GLuint					_vbo;
		GLuint					_ibo;
		GLuint					_bbo;
		GLuint					_fbo;
		uint8_t					_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
		unsigned int			_indicesSize;
		std::vector<float>		_vertices;
		std::vector<uint32_t>	_indices;
		std::vector<uint8_t>	_blockIDs;
		std::vector<uint8_t>	_faceIDs;
		World *					_world;
		std::mutex				_stateMutex;
		ChunkState				_state;

		bool					isBlockVisible(int x, int y, int z);
		bool					isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right);
};	
