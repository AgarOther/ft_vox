#pragma once

#include "ChunkMonitor.hpp"
#include "Chunk.hpp"
#include "WorldManager.hpp"
#include <cstddef>
#include <functional>
#include <unordered_map>

#define CHUNK_DELETION_DISTANCE 2
#define SEA_LEVEL 64
#define LAVA_LEVEL 25

class Player;

struct IVec2Hash
{
	size_t operator()(const glm::ivec2 & ivec2) const
	{
		return std::hash<int>()(ivec2.x) ^ (std::hash<int>()(ivec2.y) << 1);
	}
};

class World
{
	public:
		World(const std::string & name, Seed seed, Environment environment = OVERWORLD);
		~World();

		Chunk *					getChunkAt(int x, int z) const;
		Chunk *					getChunkAtChunkLocation(int x, int z) const;
		BlockType				getBlockAt(const Location & loc) const;
		const Player *			getPlayer() const;
		int						getHighestYAtChunkLocation(int x, int z) const;
		const Noise &			getNoise() const { return _noise; }
		Environment				getEnvironment() const { return _environment; }
		const std::string &		getName() const { return _name; }

		bool					isProcedural() const { return _procedural; }
		bool					isLoaded() const { return _loaded; }

		void					setProcedural(bool procedural) { _procedural = procedural; }
		void					setPlayer(Player * player);

		void					applyGravity(float deltaTime);
		void					deleteChunk(Chunk * chunk);
		void					generateProcedurally(bool firstLoad = false); // wow!
		void					render(const Shader & shader);
		
		void					load();
		void					shutdown();
	private:
		typedef std::unordered_map<glm::ivec2, Chunk * , IVec2Hash>	ChunkMap;
		typedef std::pair<float, Chunk *>							ChunkSquaredDistance;

		void					_sendToWorkers(std::vector<Chunk * > & chunks);
		void					_checkForChunkDeletion();

		ChunkMap				_chunks;
		std::string				_name;
		Player *				_player;
		ChunkMonitor			_monitor;
		bool					_procedural = true;
		Environment				_environment;
		Noise					_noise;
		bool					_loaded = false;
};
