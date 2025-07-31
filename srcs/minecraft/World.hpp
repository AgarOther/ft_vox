#pragma once

#include "fastnoiselite/FastNoiseLite.h"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"

class World
{
	public:
		World(int chunkCountX, int chunkCountZ, const TextureAtlas & atlas, const FastNoiseLite & noise);
		~World();

		void					render(const Shader & shader) const;
		Chunk *					getChunkAt(int x, int z);
	private:
		std::vector<Chunk * >	_chunks;
		// std::unordered_map<std::pair<int, int>, Chunk * >	_chunks;
};
