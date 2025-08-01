#pragma once

#include "fastnoiselite/FastNoiseLite.h"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"
#include <cstddef>
#include <functional>
#include <unordered_map>

#define GRAVITY -2.0f

class Player;

struct PairHash
{
	size_t operator()(const std::pair<int, int> & pair) const
	{
		return std::hash<int>()(pair.first) ^ (std::hash<int>()(pair.second) << 1);
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
		World(int chunkCountX, int chunkCountZ, const TextureAtlas & atlas, const FastNoiseLite & noise);
		~World();

		void					render(const Shader & shader) const;
		Chunk *					getChunkAt(int x, int z);
		Chunk *					getChunkAtChunkLocation(int x, int z);
		double					getHighestY(int x, int z);
		void					addPlayer(Player * player);
		const Player *			getPlayer(const std::string & name) const;
		void					applyGravity(float deltaTime);
	private:
		typedef std::unordered_map<std::pair<int, int>, Chunk * , PairHash> ChunkMap;
		ChunkMap				_chunks;
		typedef std::unordered_map<std::string, Player * > PlayerList;
		PlayerList				_players;
};
