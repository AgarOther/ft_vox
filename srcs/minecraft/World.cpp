#include "BlockTypeRegistry.hpp"
#include "Chunk.hpp"
#include "Frustum.hpp"
#include "Player.hpp"
#include "Location.hpp"
#include "types.hpp"
#include "utils.hpp"

void World::render(const Shader & shader, const Player & player) const
{
	glm::mat4 viewProj = player.getCamera()->getProjectionMatrix() * player.getCamera()->getViewMatrix();
	Frustum frustum(viewProj);
	for (auto & [_, chunkPtr] : _chunks)
	{
		if (chunkPtr->getState() == IDLE)
			continue;
		if (chunkPtr->getState() == GENERATED)
			chunkPtr->uploadMesh();
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

Chunk * World::getChunkAt(int x, int z) const
{
	int chunkX;
	int chunkZ;

	chunkX = floorDiv(x, CHUNK_WIDTH);
	chunkZ = floorDiv(z, CHUNK_DEPTH);
	return getChunkAtChunkLocation(chunkX, chunkZ);
}

Chunk * World::getChunkAtChunkLocation(int x, int z) const
{
	auto it = _chunks.find(std::pair<int, int>(x, z));
	if (it != _chunks.end())
		return it->second;
	return nullptr;
}

int World::getHighestY(int x, int z) const
{
	Chunk * chunk = getChunkAt(x, z);
	if (chunk)
	{
		for (int y = CHUNK_HEIGHT - 1; y >= 0; --y)
			if (chunk->getBlockAt(Location(x, y, z)).isSolid)
				return y + 1;
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

BlockType World::getBlockAt(const Location & loc) const
{
	if (loc.getY() < 0 || loc.getY() >= CHUNK_HEIGHT)
		return BlockTypeRegistry::getBlockType(AIR);
	Chunk * chunk = getChunkAt(loc.getX(), loc.getZ());
	if (!chunk || chunk->getState() == IDLE)
		return BlockTypeRegistry::getBlockType(AIR);
	return chunk->getBlockAt(loc);
}

void World::applyGravity(float deltaTime)
{
	static const float gravity = -25.0f;
	float verticalPosition;
	Location teleportLocation;

	for (auto & [_, player] : _players)
	{
		if (player->getGamemode() == SURVIVAL && (player->getVelocityY() != 0 || !player->getBlockUnder().isSolid))
		{
			verticalPosition = player->getLocation().getY() + player->getVelocityY() * deltaTime;
			player->setVelocityY(player->getVelocityY() + gravity * deltaTime);
			teleportLocation = Location(
				player->getLocation().getX(),
				verticalPosition,
				player->getLocation().getZ()
			);
			if (getBlockAt(teleportLocation).isSolid && player->getVelocityY() < 0)
			{
				Location tmp = teleportLocation.clone();
				tmp.setX(round(tmp.getX()));
				tmp.setZ(round(tmp.getZ()));
				player->setVelocityY(0);
				while (getBlockAt(tmp.add(0.0, 1.0, 0.0)).isSolid)
					;
				teleportLocation.setY(floor(tmp.getY()));
			}
			player->teleport(teleportLocation);
		}
	}
}

void World::_sendToWorkers(std::vector<Chunk * > & chunks)
{
	for (Chunk * chunk : chunks)
		_chunks[std::pair<int, int>(chunk->getChunkX(), chunk->getChunkZ())] = chunk;
	_monitor.queue(chunks);
}

void World::generateProcedurally(const Player & player, const FastNoiseLite & noise, const TextureAtlas * atlas)
{
	const Location & center = player.getLocation();
	const int centerX = std::floor(center.getX() / CHUNK_WIDTH);
	const int centerZ = std::floor(center.getZ() / CHUNK_DEPTH);
	const uint8_t renderDistance = player.getCamera()->getRenderDistance();
	std::vector<Chunk * > queue;

	for (int x = centerX + renderDistance; x >= centerX - renderDistance; x--)
	{
		for (int z = centerZ + renderDistance; z >= centerZ - renderDistance; z --)
		{
			Chunk * tmp = getChunkAt(x, z);
			if (!tmp)
				queue.push_back(new Chunk(x, z, noise, this, atlas));
		}
	}
	if (!queue.empty())
		_sendToWorkers(queue);
}

void World::shutdown()
{
	_monitor.stop();
	for (auto& [_, chunk] : _chunks)
		delete chunk;
}
