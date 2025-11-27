#pragma once

#include "ChunkMonitor.hpp"
#include "Chunk.hpp"
#include <cstddef>
#include <functional>
#include <unordered_map>

#define CHUNK_DELETION_DISTANCE 2
#define SEA_LEVEL 64
#define LAVA_LEVEL 25

class Player;

struct PlayerHash
{
	size_t operator()(const Player & player) const
	{
		return static_cast<size_t>(std::ptrdiff_t(&player));
	}
};

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
		World(TextureAtlas * atlas, const Noise & noise, Environment environment = OVERWORLD): _monitor(environment), _atlas(atlas),
			_noise(noise), _procedural(true), _environment(environment), _loaded(false) {}
		~World();

		void					render(const Shader & shader, const Player & player);

		void					deleteChunk(Chunk * chunk);
		Chunk *					getChunkAt(int x, int z) const;
		Chunk *					getChunkAtChunkLocation(int x, int z) const;
		BlockType				getBlockAt(const Location & loc) const;
		const Player *			getPlayer(const std::string & name) const;
		int						getHighestYAtChunkLocation(int x, int z) const;
		TextureAtlas *			getAtlas() const { return _atlas; }
		const Noise &			getNoise() const { return _noise; }
		bool					isProcedural() const { return _procedural; }
		Environment				getEnvironment() const { return _environment; }
		bool					isLoaded() const { return _loaded; }

		void					setProcedural(bool procedural) { _procedural = procedural; }

		void					addPlayer(Player * player);
		void					removePlayer(Player * player);
		void					applyGravity(float deltaTime);

		void					generateProcedurally(); // wow!
		
		void					load();
		void					shutdown();
	private:
		void					_sendToWorkers(std::vector<Chunk * > & chunks);

		typedef std::unordered_map<glm::ivec2, Chunk * , IVec2Hash> ChunkMap;
		ChunkMap				_chunks;
		typedef std::unordered_map<std::string, Player * > PlayerList;
		PlayerList				_players;
		ChunkMonitor			_monitor;
		TextureAtlas *			_atlas;
		Noise					_noise;
		bool					_procedural;
		Environment				_environment;
		bool					_loaded;
};
