#pragma once

#include "ChunkMonitor.hpp"
#include "Chunk.hpp"
#include "types.hpp"
#include <cstddef>
#include <functional>
#include <unordered_map>

#define CHUNK_DELETION_DISTANCE 5
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

class World
{
	public:
		World(TextureAtlas * atlas, const FastNoiseLite & noise, Environment environment = OVERWORLD): _monitor(environment), _atlas(atlas),
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
		const FastNoiseLite &	getNoise() const { return _noise; }
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

		typedef std::unordered_map<std::pair<int, int>, Chunk * , PairHash> ChunkMap;
		ChunkMap				_chunks;
		typedef std::unordered_map<std::string, Player * > PlayerList;
		PlayerList				_players;
		ChunkMonitor			_monitor;
		TextureAtlas *			_atlas;
		FastNoiseLite			_noise;
		bool					_procedural;
		std::vector<Chunk * >	_oldChunks;
		Environment				_environment;
		bool					_loaded;
};
