#include "BlockTypeRegistry.hpp"
#include "Chunk.hpp"
#include "Frustum.hpp"
#include "Player.hpp"
#include "Location.hpp"
#include "types.hpp"
#include "utils.hpp"

void World::render(const Shader & shader, const Player & player)
{
	shader.bind();
	const uint8_t renderDistance = player.getCamera()->getRenderDistance() + 1;
	glm::mat4 viewProj = player.getCamera()->getProjectionMatrix() * player.getCamera()->getViewMatrix();
	Frustum frustum(viewProj);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	player.getCamera()->setupFog(shader);
	for (auto & [_, chunkPtr] : _chunks)
	{
		if (!chunkPtr)
			continue;
		ChunkState state = chunkPtr->getState();
		if (state == IDLE)
			continue;
		if (_procedural && state == UPLOADED &&
			(chunkPtr->getChunkX() > static_cast<int>(std::floor(player.getLocation().getX() / CHUNK_WIDTH)) + renderDistance
			|| chunkPtr->getChunkX() < static_cast<int>(std::floor(player.getLocation().getX() / CHUNK_WIDTH)) - renderDistance
			|| chunkPtr->getChunkZ() > static_cast<int>(std::floor(player.getLocation().getZ() / CHUNK_DEPTH)) + renderDistance
			|| chunkPtr->getChunkZ() < static_cast<int>(std::floor(player.getLocation().getZ() / CHUNK_DEPTH)) - renderDistance))
		{
			chunkPtr->unloadMesh();
			if (std::find(_oldChunks.begin(), _oldChunks.end(), chunkPtr) == _oldChunks.end())
				_oldChunks.push_back(chunkPtr);
			continue;
		}
		if (state == MESHED)
			chunkPtr->uploadMesh();
		if (state == CLEANED)
		{
			chunkPtr->unloadMesh();
			chunkPtr->uploadMesh();
		}
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
	for (auto it = _oldChunks.begin(); it != _oldChunks.end(); ++it)
	{
		Chunk * chunk = *it;
		if (chunk && player.getLocation().distance(Location(chunk->getChunkX(), static_cast<int>(player.getLocation().getY()), chunk->getChunkZ()))
				>= player.getCamera()->getRenderDistance() * std::max(CHUNK_WIDTH, CHUNK_DEPTH) * CHUNK_DELETION_DISTANCE)
		{
			deleteChunk(chunk);
			it = _oldChunks.erase(it) - 1;
		}
	}
}

void World::deleteChunk(Chunk * chunk)
{
	auto it = _chunks.find(chunk->getChunkLocation());
	if (it != _chunks.end())
	{
		_chunks.erase(it);
		delete chunk;
	}
}

Chunk * World::getChunkAt(int x, int z) const
{
	int chunkX;
	int chunkZ;

	chunkX = std::floor(static_cast<double>(x) / CHUNK_WIDTH);
	chunkZ = std::floor(static_cast<double>(z) / CHUNK_DEPTH);
	return getChunkAtChunkLocation(chunkX, chunkZ);
}

Chunk * World::getChunkAtChunkLocation(int x, int z) const
{
	auto it = _chunks.find(std::pair<int, int>(x, z));
	if (it != _chunks.end())
		return it->second;
	return nullptr;
}

int World::getHighestYAtChunkLocation(int x, int z) const
{
	const int chunkX = x - (x % CHUNK_WIDTH);
	const int chunkZ = z - (z % CHUNK_DEPTH);
	Chunk * chunk = getChunkAtChunkLocation(chunkX, chunkZ);
	if (chunk)
	{
		for (int y = CHUNK_HEIGHT - 1; y >= 0; --y)
		{
			BlockType block = chunk->getBlockAtChunkLocation(Location(chunkX, y, chunkZ));
			if (block.isSolid && block.type != BEDROCK)
				return y + 1;
		}
	}
	std::cout << "[Warning] Requesting unknown chunk at (" << x << ", " << z << ")." << std::endl;
	return 0;
}

int World::getHighestY(int x, int z) const
{
	Chunk * chunk = getChunkAt(x, z);
	if (chunk)
	{
		for (int y = CHUNK_HEIGHT - 1; y >= 0; --y)
		{
			BlockType block = chunk->getBlockAt(Location(x, y, z));
			if (block.isSolid && block.type != BEDROCK)
				return y + 1;
		}
	}
	std::cout << "[Warning] Requesting unknown chunk at (" << x << ", " << z << ")." << std::endl;
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
	Chunk * chunk = getChunkAt(static_cast<int>(loc.getX()), static_cast<int>(loc.getZ()));
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
				player->setVelocityY(0);

				Location tmp = teleportLocation.clone();
				tmp.setX(std::round(tmp.getX()));
				tmp.setZ(std::round(tmp.getZ()));
				while (getBlockAt(tmp.add(0.0, 1.0, 0.0)).isSolid && tmp.getY() < CHUNK_HEIGHT)
					;
				teleportLocation.setY(std::floor(tmp.getY()));
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

// next step is to have new chunks mark neighboring chunks as dirty to rebuild mesh flawlessly by preparing new vbo/vao and then swapping the new with old
void World::generateProcedurally()
{
	static long cooldown = 0;
	if (!_procedural || _monitor.areWorkersWorking() || (cooldown && getTimeAsMilliseconds() - cooldown < 100))
		return;
	cooldown = getTimeAsMilliseconds();

	for (auto it = _players.begin(); it != _players.end(); ++it)
	{
		Player * player = it->second;
		const Location & center = player->getLocation();
		const int centerX = std::floor(center.getX() / CHUNK_WIDTH);
		const int centerZ = std::floor(center.getZ() / CHUNK_DEPTH);
		const uint8_t renderDistance = player->getCamera()->getRenderDistance();
		std::vector<Chunk * > queue;

		for (int x = centerX - renderDistance; x < centerX + renderDistance; x++)
		{
			for (int z = centerZ - renderDistance; z < centerZ + renderDistance; z++)
			{
				Chunk * tmp = getChunkAtChunkLocation(x, z);
				if (!tmp || tmp->getState() == GENERATED)
				{
					if (tmp)
					{
						for (Chunk * chunk : tmp->getNeighborChunks())
						{
							if (chunk && chunk->getState() == UPLOADED)
							{
								chunk->setState(DIRTY);
								queue.push_back(chunk);
							}
						}
					}
					queue.push_back(!tmp ? new Chunk(x, z, this) : tmp);
				}
			}
		}
		// Pregenerating chunks that are far from the player when workers aren't active
		if (queue.empty() && !_monitor.areWorkersWorking())
		{
			for (int x = centerX - (renderDistance * 1.5); x < centerX + (renderDistance * 1.5); x++)
			{
				for (int z = centerZ - (renderDistance * 1.5); z < centerZ + (renderDistance * 1.5); z++)
				{
					if (x >= centerX - renderDistance && x < centerX + renderDistance &&
							z >= centerZ - renderDistance && z < centerZ + renderDistance)
						continue;
					Chunk * tmp = getChunkAtChunkLocation(x, z);
					if (!tmp)
						queue.push_back(new Chunk(x, z, this));
				}
			}
		}
		if (!queue.empty())
			_sendToWorkers(queue);
	}
}

void World::shutdown()
{
	_monitor.stop();
	for (auto& [_, chunk] : _chunks)
		delete chunk;
}
