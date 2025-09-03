#pragma once

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "Noise.hpp"
#include "Location.hpp"
#include "types.hpp"
#include <cstdint>
#include <mutex>

#define CHUNK_HEIGHT 256
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define VERTICES_COUNT 8
#define FREQUENCY 0.00522
#define AMPLITUDE 0.00261
#define OCTAVES 5

class World;

enum ChunkState
{
	IDLE,
	GENERATED,
	DIRTY,
	CLEANED,
	MESHED,
	UPLOADED
};

class Chunk
{
	public:
		Chunk(int chunkX, int chunkZ, World * world):
			_chunkX(chunkX), _chunkZ(chunkZ), _vao(0), _vbo(0), _ibo(0), _tbo(0), _indicesSize(0),
			 _world(world), _state(IDLE) {}
		~Chunk();

		void						generateBlocks(Environment environment);
		void						generateMesh();
		void						uploadMesh();
		void						unloadMesh();
		void						render(const Shader & shader) const;
		BlockType					getBlockAt(const Location & loc);
		BlockType					getBlockAtChunkLocation(const Location & loc);
		std::vector<Chunk * >		getNeighborChunks();
		void						changeBlockAt(const Location & loc, Material newMaterial);

		int							getChunkX() const { return _chunkX; }
		int							getChunkZ() const { return _chunkZ; }
		std::pair<int, int>			getChunkLocation() const { return { _chunkX, _chunkZ }; }
		ChunkState					getState() { const std::lock_guard<std::mutex> lg(_stateMutex); return _state; }

		void						setState(ChunkState state) { const std::lock_guard<std::mutex> lg(_stateMutex); _state = state; }
	private:
		const int					_chunkX;
		const int					_chunkZ;
		GLuint						_vao;
		GLuint						_vbo;
		GLuint						_ibo;
		GLuint						_tbo;
		uint8_t						_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
		unsigned int				_indicesSize;
		std::vector<float>			_vertices;
		std::vector<uint16_t>		_indices;
		std::vector<glm::u8vec3>	_tints;
		World *						_world;
		std::mutex					_stateMutex;
		ChunkState					_state;

		bool					_isBlockVisible(int x, int y, int z);
		bool					_isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right);
		void					_generateStructures();
		void					_generateSand(Environment environment);
};	

std::ostream &					operator<<(std::ostream & os, Chunk & chunk);
