#pragma once

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "Noise.hpp"
#include "Location.hpp"
#include "types.hpp"
#include <atomic>
#include <cstdint>
#include <mutex>

#define VERTICES_COUNT	8
#define CHUNK_HEIGHT	256
#define CHUNK_WIDTH		16
#define CHUNK_DEPTH		16
#define FREQUENCY		0.00522f
#define AMPLITUDE		1.00261f
#define OCTAVES			5

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

struct Block
{
	Location	position;
	BlockType	blockType;
};

class Chunk
{
	public:
		Chunk(int chunkX, int chunkZ, World * world):
			_chunkX(chunkX), _chunkZ(chunkZ), _world(world), _state(IDLE) {}
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
		glm::ivec2					getChunkLocation() const { return { _chunkX, _chunkZ }; }
		ChunkState					getState() { const std::lock_guard<std::mutex> lg(_stateMutex); return _state; }
		bool						hasPriority() const { return _hasWorkerPriority; }

		void						setState(ChunkState state) { const std::lock_guard<std::mutex> lg(_stateMutex); _state = state; }
		void						setPriority(bool priority) { _hasWorkerPriority = priority; }
	
		void						addToPriorityMeshing();
	private:
		typedef std::unordered_map<glm::ivec2, float> NoiseCache;
		const int					_chunkX;
		const int					_chunkZ;
		GLuint						_vao = 0;
		GLuint						_vbo = 0;
		GLuint						_ibo = 0;
		uint8_t						_blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
		size_t						_indicesSize = 0;
		std::vector<float>			_vertices;
		std::vector<uint16_t>		_indices;
		World *						_world;
		std::mutex					_stateMutex;
		std::mutex					_generateMutex;
		std::mutex					_meshMutex;
		ChunkState					_state;
		int							_highestY = 0;
		std::atomic_bool			_hasWorkerPriority = false;

		bool						_isBlockVisible(int x, int y, int z);
		bool						_isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right);
		void						_generateStructures();
		void						_generateSand(Environment environment);
};	

std::ostream &					operator<<(std::ostream & os, Chunk & chunk);
