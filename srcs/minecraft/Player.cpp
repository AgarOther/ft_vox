#include "World.hpp"
#include "glm/glm.hpp"
#include "utils.hpp"
#include "BlockTypeRegistry.hpp"
#include <Chunk.hpp>
#include <cmath>
#include <cstdlib>
#include <unordered_map>

Player::Player(const std::string & name, Camera * camera, World * world)
{
	_name = name;
	_health = 20;
	_world = world;
	_camera = camera;
	_spawnLocation = Location(0.5, 0.0, 0.5);
	_location = _spawnLocation.clone();
	_boundingBox = BoundingBox(Location(0, 0, 0), Location(1, 2, 1));
	_gamemode = SURVIVAL;
	_velocity = glm::vec3(0.0f);
	_spawned = false;
	_blockInHand = GRASS_BLOCK;
	_camera->setFogColor(_camera->getFogColorByEnvironment(_world->getEnvironment()));

	_world->setPlayer(this);
	_world->load();
}

void Player::checkIfSpawned()
{
	if (!_spawned)
	{
		Chunk * spawnChunk = _world->getChunkAt(_spawnLocation.getX(), _spawnLocation.getZ());
		if (spawnChunk && spawnChunk->getState() >= GENERATED)
		{
			_spawnLocation.setY(_world->getHighestYAtChunkLocation(_spawnLocation.getX(), _spawnLocation.getZ())); // make a function to chunkalize coords
			teleport(_spawnLocation);
			_spawned = true;
		}
		else
			return;
	}
}

void Player::teleport(const Location & location)
{
	 _location = location;
	 _camera->setPosition(location.clone().add(0.0, CAMERA_OFFSET_Y, 0.0).clone().getVec3());
}

Block Player::getTargetedBlock() const
{
	// GPT-Generated
    const float maxDist = 4.5f;
    const glm::vec3 rayOrigin = _camera->getPosition();
    const glm::vec3 rayDir = glm::normalize(_camera->getOrientation());
    glm::ivec3 voxel = glm::floor(rayOrigin);
    const glm::ivec3 step(
        (rayDir.x > 0) ? 1 : -1,
        (rayDir.y > 0) ? 1 : -1,
        (rayDir.z > 0) ? 1 : -1
    );
    const glm::vec3 invRayDir(
        rayDir.x == 0 ? 1e30f : 1.0f / rayDir.x,
        rayDir.y == 0 ? 1e30f : 1.0f / rayDir.y,
        rayDir.z == 0 ? 1e30f : 1.0f / rayDir.z
    );
    const glm::vec3 nextBoundary(
        voxel.x + (step.x > 0 ? 1.0f : 0.0f),
        voxel.y + (step.y > 0 ? 1.0f : 0.0f),
        voxel.z + (step.z > 0 ? 1.0f : 0.0f)
    );
    const glm::vec3 tDelta = glm::abs(invRayDir);
    glm::vec3 tMax = (nextBoundary - rayOrigin) * invRayDir;
    float dist = 0.0f;
	BlockType hit;

    while (dist <= maxDist)
    {
		hit = _world->getBlockAt(Location(voxel));
        if (hit.isSolid && !hit.isLiquid)
            return Block{ Location(voxel), hit };
        if (tMax.x < tMax.y)
        {
            if (tMax.x < tMax.z)
            {
                voxel.x += step.x;
                dist = tMax.x;
                tMax.x += tDelta.x;
            }
            else
            {
                voxel.z += step.z;
                dist = tMax.z;
                tMax.z += tDelta.z;
            }
        }
        else
        {
            if (tMax.y < tMax.z)
            {
                voxel.y += step.y;
                dist = tMax.y;
                tMax.y += tDelta.y;
            }
            else
            {
                voxel.z += step.z;
                dist = tMax.z;
                tMax.z += tDelta.z;
            }
        }
    }

    return Block{ Location(voxel), hit };
}

BlockType Player::getBlockAtEyeLocation() const
{
	return _world->getBlockAt(Location(_camera->getPosition()));
}

BlockType Player::getBlockUnder(int xOffset, int yOffset, int zOffset) const
{
	if (getLocation().getY() >= CHUNK_HEIGHT || getLocation().getY() < 0)
		return BlockTypeRegistry::getBlockType(AIR);
	Location blockLocation = getLocation().clone();
	blockLocation.setX(std::floor(blockLocation.getX() - xOffset));
	blockLocation.setY(std::ceil(blockLocation.getY() - yOffset - 1));
	blockLocation.setZ(std::floor(blockLocation.getZ() - zOffset));
	return _world->getBlockAt(blockLocation);
}

void Player::setWorld(World * world)
{
	_world->setPlayer(nullptr);
	_world->shutdown();

	world->load();
	world->setPlayer(this);

	_world = world;
}

void Player::checkFogChange()
{
	if (getBlockAtEyeLocation().material == LAVA)
	{
		_camera->setFogStart(0.005f);
		_camera->setFogEnd(0.015f);
		_camera->setFogColor(glm::vec3(FOG_COLOR_NETHER));
	}
	else if (getBlockAtEyeLocation().material == WATER)
	{
		_camera->setFogStart(0.08f);
		_camera->setFogEnd(0.5f);
		_camera->setFogColor(glm::vec3(FOG_COLOR_WATER));
	}
	else
	{
		_camera->setFogStart(FOG_START);
		_camera->setFogEnd(FOG_END);
		_camera->setFogColor(_camera->getFogColorByEnvironment(_world->getEnvironment()));
	}
}

bool Player::isWithinRenderDistance(Chunk * chunk) const
{
	return !(chunk->getChunkX() > static_cast<int>(std::floor(getLocation().getX() / CHUNK_WIDTH)) + _camera->getRenderDistance()
	|| chunk->getChunkX() < static_cast<int>(std::floor(getLocation().getX() / CHUNK_WIDTH)) - _camera->getRenderDistance()
	|| chunk->getChunkZ() > static_cast<int>(std::floor(getLocation().getZ() / CHUNK_DEPTH)) + _camera->getRenderDistance()
	|| chunk->getChunkZ() < static_cast<int>(std::floor(getLocation().getZ() / CHUNK_DEPTH)) - _camera->getRenderDistance());
}
