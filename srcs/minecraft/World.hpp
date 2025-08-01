#pragma once

#include "fastnoiselite/FastNoiseLite.h"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"
#include <cstddef>
#include <functional>

struct PairHash
{
	size_t operator()(const std::pair<int, int> & pair) const
	{
		return std::hash<int>()(pair.first) ^ (std::hash<int>()(pair.second) << 1);
	}
};

class World
{
	public:
		World(int chunkCountX, int chunkCountZ, const TextureAtlas & atlas, const FastNoiseLite & noise);
		~World();

		void					render(const Shader & shader) const;
		Chunk *					getChunkAt(int x, int z);
	private:
		typedef std::unordered_map<std::pair<int, int>, Chunk * , PairHash> ChunkMap;
		ChunkMap				_chunks;
};
