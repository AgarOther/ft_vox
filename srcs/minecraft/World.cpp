#include "Frustum.hpp"
#include "Player.hpp"
#include "Chunk.hpp"
#include "Location.hpp"
#include "types.hpp"

World::World(int chunkCountX, int chunkCountZ, const TextureAtlas & atlas, const FastNoiseLite & noise)
{
	for (int x = 0; x < chunkCountX; x++)
	{
		for (int z = 0; z < chunkCountZ; z++)
		{
			Chunk * chunk = new Chunk(x, z, noise);
			_chunks[std::pair<int, int>(x, z)] = chunk;
		}
	}
	for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
		it->second->generateMesh(atlas, this);
}

World::~World()
{
	for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
		delete it->second;
}

void World::render(const Shader & shader, const Player & player) const
{
	shader.bind();
	// for (auto it = _chunks.begin(); it != _chunks.end(); ++it)
	// 	if (it->second)
	// 		it->second->render(shader);
	glm::mat4 viewProj = player.getCamera()->getProjectionMatrix() * player.getCamera()->getViewMatrix();
	Frustum frustum(viewProj);
	for (auto & [chunkKey, chunkPtr] : _chunks)
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

	chunkX = (x - (x % 16)) / 16;
	chunkZ = (z - (z % 16)) / 16;
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

void World::applyGravity(float deltaTime)
{
	double newPlayerY;

	for (auto it = _players.begin(); it != _players.end(); ++it)
	{
		Player * player = it->second;
		if (player->getVelocityY() != 0)
		{
			newPlayerY = player->getLocation().getY() + -player->getVelocityY() * deltaTime;
			Chunk * chunk = getChunkAt(player->getLocation().getX(), player->getLocation().getZ());
			if (!chunk || !chunk->getBlockAt(Location(player->getLocation().getX(),
				std::floor(newPlayerY),
				player->getLocation().getZ())).isSolid)
			{
				player->teleport(Location(
					player->getLocation().getX(),
					std::floor(newPlayerY) + 1.0,
					player->getLocation().getZ()
				));
				player->setVelocityY(0);
				continue;
			}
			player->setVelocityY(player->getVelocityY() + GRAVITY * deltaTime);
			player->teleport(Location(
				player->getLocation().getX(),
				newPlayerY,
				player->getLocation().getZ()
			));
		}
	}
}
