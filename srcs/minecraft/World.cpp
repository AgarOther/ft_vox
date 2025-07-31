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
			_chunks.push_back(chunk);
		}
	}
}

World::~World()
{
	for (Chunk * chunk : _chunks)
		delete chunk;
}

void World::render(const Shader & shader) const
{
	shader.bind();
	for (Chunk * chunk : _chunks)
		chunk->render(shader);
}

Chunk * World::getChunkAt(int x, int z)
{
	int chunkX;
	int chunkZ;

	chunkX = x - (x % 16) - 1;
	chunkZ = z - (z % 16) - 1;
	for (Chunk * chunk : _chunks)
	{
		if (chunk->getChunkX() == chunkX && chunk->getChunkZ() == chunkZ)
			return chunk;
	}
	return nullptr;
}
