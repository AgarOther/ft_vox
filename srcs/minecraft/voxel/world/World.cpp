#include "BlockTypeRegistry.hpp"
#include "Chunk.hpp"
#include "Frustum.hpp"
#include "Player.hpp"
#include "Location.hpp"
#include "utils.hpp"
#include "colors.hpp"
#include <algorithm>

World::World(const std::string & name, Seed seed, Environment environment): _name(name), _monitor(environment), _environment(environment),
	_noise(Noise(seed, FREQUENCY, AMPLITUDE))
{

}

void World::load()
{
	if (_loaded)
	{
		std::cerr << YELLOW << "[Warning] Tried to load an already loaded world." << RESET << std::endl;
		return;
	}
	_monitor.start();
	_loaded = true;
}

void World::render(const Shader & shader)
{
	if (!_loaded || !_player)
		return;
	std::vector<ChunkSquaredDistance> renderChunks;

	_player->checkIfSpawned();
	std::vector<glm::ivec2> deletableChunks;
	shader.bind();
	const uint8_t renderDistance = _player->getCamera()->getRenderDistance() + 1;
	glm::mat4 viewProj = _player->getCamera()->getProjectionMatrix() * _player->getCamera()->getViewMatrix();
	Frustum frustum(viewProj);
	_player->getCamera()->setupFog(shader);
	for (auto & [chunkPos, chunkPtr] : _chunks)
	{
		if (!chunkPtr)
			continue;

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
		
		ChunkState state = chunkPtr->getState();
		if (state == IDLE)
			continue;
		if (_procedural && state == UPLOADED &&
			(chunkPtr->getChunkX() > static_cast<int>(std::floor(_player->getLocation().getX() / CHUNK_WIDTH)) + renderDistance * CHUNK_DELETION_DISTANCE
			|| chunkPtr->getChunkX() < static_cast<int>(std::floor(_player->getLocation().getX() / CHUNK_WIDTH)) - renderDistance * CHUNK_DELETION_DISTANCE
			|| chunkPtr->getChunkZ() > static_cast<int>(std::floor(_player->getLocation().getZ() / CHUNK_DEPTH)) + renderDistance * CHUNK_DELETION_DISTANCE
			|| chunkPtr->getChunkZ() < static_cast<int>(std::floor(_player->getLocation().getZ() / CHUNK_DEPTH)) - renderDistance * CHUNK_DELETION_DISTANCE))
		{
			delete chunkPtr;
			deletableChunks.push_back(chunkPos);
			continue;
		}
		if (!frustum.isChunkVisible(min, max))
			continue;
		if (state == MESHED)
			chunkPtr->uploadMesh();
		if (state == CLEANED)
		{
			chunkPtr->unloadMesh();
			chunkPtr->uploadMesh();
		}
		if (_player->isWithinRenderDistance(chunkPtr))
		{
			// big thanks to GPT for this
			glm::vec3 chunkCenter = (min + max) * 0.5f;
			glm::vec3 diff = chunkCenter - _player->getCamera()->getPosition();
			float sqDist = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

			renderChunks.push_back({ sqDist, chunkPtr });
		}
	}

	std::sort(renderChunks.begin(), renderChunks.end(), [](const ChunkSquaredDistance & a, const ChunkSquaredDistance & b)
	{
		return a.first > b.first;
	});

	for (const ChunkSquaredDistance & chunkInfo : renderChunks)
		chunkInfo.second->render(shader);

	for (glm::ivec2 chunkPos : deletableChunks)
		_chunks.erase(chunkPos);
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

	chunkX = static_cast<int>(std::floor(static_cast<double>(x) / CHUNK_WIDTH));
	chunkZ = static_cast<int>(std::floor(static_cast<double>(z) / CHUNK_DEPTH));
	return getChunkAtChunkLocation(chunkX, chunkZ);
}

Chunk * World::getChunkAtChunkLocation(int x, int z) const
{
	auto it = _chunks.find(glm::ivec2(x, z));
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
		for (int y = CHUNK_HEIGHT - 2; y >= 0; --y)
		{
			BlockType block = chunk->getBlockAtChunkLocation(Location(chunkX, y, chunkZ));
			if (block.isSolid && block.material != BEDROCK &&
					(y + 1 < CHUNK_HEIGHT && !chunk->getBlockAtChunkLocation(Location(chunkX, y + 1, chunkZ)).isSolid)
				 	&& (y + 2 < CHUNK_HEIGHT && !chunk->getBlockAtChunkLocation(Location(chunkX, y + 2, chunkZ)).isSolid))
				return y + 2;
		}
	}
	std::cout << "[Warning] Requesting unknown chunk at (" << x << ", " << z << ")." << std::endl;
	return 0;
}

void World::setPlayer(Player * player)
{
	_player = player;
}

const Player * World::getPlayer() const
{
	return _player;
}

BlockType World::getBlockAt(const Location & loc) const
{
	if (loc.getY() < 0 || loc.getY() >= CHUNK_HEIGHT)
		return BlockTypeRegistry::getBlockType(AIR);
	Chunk * chunk = getChunkAt(static_cast<int>(std::floor(loc.getX())), static_cast<int>(std::floor(loc.getZ())));
	if (!chunk || chunk->getState() == IDLE)
		return BlockTypeRegistry::getBlockType(AIR);
	return chunk->getBlockAt(loc);
}

void World::applyGravity(float deltaTime)
{
	if (!_loaded || !_player)
		return;

	static const float gravity = -25.0f;
	float verticalPosition;
	Location teleportLocation;
	Material playerStandingBlock = _player->getBlockUnder(0, -1, 0).material;
	const float liquidModifier = playerStandingBlock == WATER ? 0.5f : playerStandingBlock == LAVA ? 3.0f : 1.0f;

	if (_player->getGamemode() == SURVIVAL && (_player->getVelocityY() != 0 || !_player->getBlockUnder().isSolid))
	{
		verticalPosition = _player->getLocation().getY() + _player->getVelocityY() * deltaTime;
		_player->setVelocityY(_player->getVelocityY() + gravity * liquidModifier * deltaTime);
		teleportLocation = Location(
			_player->getLocation().getX(),
			verticalPosition,
			_player->getLocation().getZ()
		);
		if (getBlockAt(_player->getEyeLocation().clone().add(0.0, 0.5, 0.0)).isSolid && _player->getVelocityY() > 0)
			_player->setVelocityY(0);
		if (getBlockAt(teleportLocation).isSolid && _player->getVelocityY() < 0)
		{
			_player->setVelocityY(0);

			Location tmp = teleportLocation.clone();
			tmp.setX(std::floor(tmp.getX()));
			tmp.setZ(std::floor(tmp.getZ()));
			while (getBlockAt(tmp.add(0.0, 1.0, 0.0)).isSolid && tmp.getY() < CHUNK_HEIGHT)
				;
			teleportLocation.setY(std::floor(tmp.getY()));
		}
		_player->teleport(teleportLocation);
		_player->checkFogChange();
	}
}

void World::_sendToWorkers(std::vector<Chunk * > & chunks)
{
	for (Chunk * chunk : chunks)
	{
		ChunkState state = chunk->getState();
		if (state == UPLOADED || state == CLEANED)
			continue;
		_chunks[glm::ivec2(chunk->getChunkX(), chunk->getChunkZ())] = chunk;
	}
	_monitor.queue(chunks);
}

void World::generateProcedurally()
{
	static long cooldown = 0;
	if (!_loaded || !_procedural || !_player || _monitor.areWorkersWorking() || (cooldown && getTimeAsMilliseconds() - cooldown < 100))
		return;
	cooldown = getTimeAsMilliseconds();

	const Location & center = _player->getLocation();
	const int centerX = std::floor(center.getX() / CHUNK_WIDTH);
	const int centerZ = std::floor(center.getZ() / CHUNK_DEPTH);
	const uint8_t renderDistance = _player->getCamera()->getRenderDistance();
	std::vector<Chunk * > queue;

	for (int x = centerX - renderDistance; x < centerX + renderDistance; x++)
	{
		for (int z = centerZ - renderDistance; z < centerZ + renderDistance; z++)
		{
			Chunk * tmp = getChunkAtChunkLocation(x, z);
			if (tmp)
			{
				ChunkState state = tmp->getState();
				if (state == GENERATED)
				{
					for (Chunk * chunk : tmp->getNeighborChunks())
					{
						if (chunk && chunk->getState() >= MESHED)
						{
							chunk->setState(DIRTY);
							queue.push_back(chunk);
						}
					}
				}
				else if (state == DIRTY)
					queue.push_back(tmp);
			}
			queue.push_back(!tmp ? new Chunk(x, z, this) : tmp);
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
		_sendToWorkers(queue); // sort before?
}

void World::shutdown()
{
	_monitor.stop();
	_loaded = false;
}

World::~World()
{
	for (auto& [_, chunk] : _chunks)
		delete chunk;
}
