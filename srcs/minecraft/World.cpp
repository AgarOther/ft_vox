#include "World.hpp"
#include <Chunk.hpp>

World::World(int chunkCountX, int chunkCountZ, const TextureAtlas & atlas, const FastNoiseLite & noise)
{
	for (int x = 0; x < chunkCountX; x++)
	{
		for (int z = 0; z < chunkCountZ; z++)
		{
			Chunk * chunk = new Chunk(x, z, noise);
			chunk->generateMesh(atlas);
			_chunks[std::pair<int, int>(x, z)] = chunk;
		}
	}
}

World::~World()
{
	for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
		delete it->second;
}

void World::render(const Shader & shader) const
{
	shader.bind();
	for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
		it->second->render(shader);
}

Chunk * World::getChunkAt(int x, int z)
{
	int chunkX;
	int chunkZ;

	chunkX = (x - (x % 16)) / 16;
	chunkZ = (z - (z % 16)) / 16;
	for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
	{
		if (it->second->getChunkX() == chunkX && it->second->getChunkZ() == chunkZ)
			return it->second;
	}
	return nullptr;
}
