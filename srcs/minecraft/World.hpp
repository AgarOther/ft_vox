#pragma once

#include "ChunkMonitor.hpp"
#include "Chunk.hpp"
#include <cstddef>
#include <functional>
#include <unordered_map>

class Player;

struct PairHash
{
	size_t operator()(const std::pair<int, int> & pair) const
	{
		size_t h1 = std::hash<int>{}(pair.first);
		size_t h2 = std::hash<int>{}(pair.second);
		return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
	}
};

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
		World(TextureAtlas * atlas, const FastNoiseLite & noise): _atlas(atlas), _noise(noise) {};

		void					render(const Shader & shader, const Player & player) const;

		Chunk *					getChunkAt(int x, int z) const;
		Chunk *					getChunkAtChunkLocation(int x, int z) const;
		BlockType				getBlockAt(const Location & loc) const;
		const Player *			getPlayer(const std::string & name) const;
		int						getHighestY(int x, int z) const;
		TextureAtlas *			getAtlas() const { return _atlas; }
		const FastNoiseLite &	getNoise() const { return _noise; }

		void					addPlayer(Player * player);
		void					applyGravity(float deltaTime);

		void					generateProcedurally(); // wow!
		
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
};
