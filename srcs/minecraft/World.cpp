#include "BlockTypeRegistry.hpp"
#include "Chunk.hpp"
#include "Frustum.hpp"
#include "Player.hpp"
#include "Location.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <future>

World::World(int chunkCountX, int chunkCountZ, const TextureAtlas & atlas, const FastNoiseLite & noise)
{
	std::vector<std::future<void>> tasks;

	// Allocates chunks
	for (int x = 0; x < chunkCountX; x++)
	{
		for (int z = 0; z < chunkCountZ; z++)
		{
			Chunk * chunk = new Chunk(x, z, noise);
			_chunks[std::pair<int, int>(x, z)] = chunk;
		}
	}

	// Generate blocks in chunks
	for (auto & pair : _chunks)
	{
		Chunk* chunk = pair.second;
		tasks.push_back(std::async(std::launch::async, [chunk]() {
			chunk->generateBlocks();
		}));
	}
	for (auto& task : tasks)
		task.get();
	tasks.clear();

	// Generate meshes for chunks
	for (auto & pair : _chunks)
	{
		Chunk* chunk = pair.second;
		tasks.push_back(std::async(std::launch::async, [chunk, &atlas, this]() {
			chunk->generateMesh(atlas, this);
		}));
	}
	for (auto & task : tasks)
		task.get();

	// Upload to OpenGL (must be main thread)
	for (auto & pair : _chunks)
	{
		Chunk* chunk = pair.second;
		chunk->uploadMesh();
	}
}

World::~World()
{
	for (auto& [_, chunk] : _chunks)
		delete chunk;
}

void World::render(const Shader & shader, const Player & player) const
{
	glm::mat4 viewProj = player.getCamera()->getProjectionMatrix() * player.getCamera()->getViewMatrix();
	Frustum frustum(viewProj);
	for (auto & [_, chunkPtr] : _chunks)
	{
		const int chunkX = chunkPtr->getChunkX();
		const int chunkZ = chunkPtr->getChunkZ();
		glm::vec3 min = {
			chunkX * CHUNK_WIDTH,
			0,
			chunkZ * CHUNK_DEPTH
		};
		glm::vec3 max = {
			min.x + CHUNK_WIDTH,
			CHUNK_HEIGHT,
			min.z + CHUNK_DEPTH
		};
		if (frustum.isChunkVisible(min, max))
			chunkPtr->render(shader);
	}
}

Chunk * World::getChunkAt(int x, int z)
{
	int chunkX;
	int chunkZ;

	chunkX = floorDiv(x, CHUNK_WIDTH);
	chunkZ = floorDiv(z, CHUNK_DEPTH);
	return getChunkAtChunkLocation(chunkX, chunkZ);
}

Chunk * World::getChunkAtChunkLocation(int x, int z)
{
	auto it = _chunks.find(std::pair<int, int>(x, z));
	if (it != _chunks.end())
		return it->second;
	return nullptr;
}

double World::getHighestY(int x, int z)
{
	Chunk * chunk = getChunkAt(x, z);
	if (chunk)
	{
		for (int y = CHUNK_HEIGHT - 1; y >= 0; --y)
			if (chunk->getBlockAt(Location(x, y, z)).isSolid)
				return y;
	}
	return 0;
}

void World::addPlayer(Player * player)
{
	_players[player->getName()] = player;
}

const Player * World::getPlayer(const std::string & name) const
{
	auto it = _players.find(name);
	if (it != _players.end())
		return it->second;
	return nullptr;
}

BlockType World::getBlockAt(const Location & loc)
{
	if (loc.getY() < 0 || loc.getY() >= CHUNK_HEIGHT)
		return BlockTypeRegistry::getBlockType(AIR);
	Chunk * chunk = getChunkAt(loc.getX(), loc.getZ());
	if (!chunk)
		return BlockTypeRegistry::getBlockType(AIR);
	return chunk->getBlockAt(loc);
}

void World::applyGravity(float deltaTime)
{
	float baseVelocity = -3.5f * deltaTime;
	float maxVelocity = -150.f * deltaTime;

	for (auto & [_, player] : _players)
	{
		if (player->getGamemode() == SURVIVAL && !player->getBlockUnder().isSolid && player->getVelocityY() <= 0)
		{
			if (player->getVelocityY() == 0)
				player->setVelocityY(baseVelocity);
			else if (player->getVelocityY() > maxVelocity)
				player->setVelocityY(player->getVelocityY() + (player->getVelocityY() * 5 * deltaTime));
			player->teleport(player->getLocation().clone().add(0.0, player->getVelocityY(), 0.0));
			if (player->getBlockUnder(-player->getVelocityY()).isSolid)
			{
				// High fall can glitch, fix by checking current Y against next Y + getVelocity(), basically checking its next call.
				// if theres a solid block inbetween, fall on it. Only do that if distance is less than the velocity.
				player->teleport(Location(player->getLocation().getX(), ceil(player->getLocation().getY()) + ceil(player->getVelocityY()), player->getLocation().getZ()));
				player->setVelocityY(0);
			}
		}
		else if (player->getGamemode() == SURVIVAL && player->getVelocityY() > 0)
		{
			player->setVelocityY(player->getVelocityY() - (player->getVelocityY() * 5 * deltaTime));
			player->teleport(player->getLocation().clone().add(0.0, player->getVelocityY(), 0.0));
			if (player->getVelocityY() <= 2.3f * deltaTime)
				player->setVelocityY(-(2.3f * deltaTime));
		}
	}
}
