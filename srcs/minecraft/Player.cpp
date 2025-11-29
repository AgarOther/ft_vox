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

	_world->setPlayer(this);
	_world->load();
}

static glm::vec3 translateDirection(const float yaw, const float pitch)
{
	glm::vec3 direction;
	direction.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	direction.y = sinf(glm::radians(pitch));
	direction.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	return (direction);
}

void Player::interceptInputs(GLFWwindow * window, float deltaTime)
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

	static bool lastFrameMouseClicked = false;
	const bool mouseClickedL = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (mouseClickedL && !lastFrameMouseClicked)
	{
		Block block = getTargetedBlock();
		if (block.blockType.type != AIR)
		{
			const Location position = getTargetedBlock().position;
			Chunk * chunk = _world->getChunkAt(position.getX(), position.getZ());
			if (chunk)
				chunk->changeBlockAt(getTargetedBlock().position, AIR);
		}
	}
	lastFrameMouseClicked = mouseClickedL;
	
	Location finalLocation = getLocation();

	/* GPT Code */
	// GPT my friend who helps me with the weirdest maths
	// Calculate forward vector for movement based on pitch (only in X-Z plane)
	glm::vec3 forward;
	forward.x = cosf(glm::radians(_camera->getPitch())) * cosf(glm::radians(_camera->getYaw())); // X component based on pitch
	forward.z = cosf(glm::radians(_camera->getPitch())) * sinf(glm::radians(_camera->getYaw())); // Z component based on pitch
	forward.y = 0.0f; // No vertical movement based on pitch, set Y to 0

	// Normalize the forward vector to maintain consistent speed
	forward = glm::normalize(forward);

	// Calculate the right vector (strafe) using yaw, no pitch influence
	const glm::vec3 right = glm::normalize(glm::cross(forward, _camera->getAltitude())); // Right direction is based on forward and altitude (up vector)
	/* End GPT Code */

	bool isWindowFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

	if (!_camera->isLocked() && isWindowFocused)
	{
		// Key management
		bool shiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

		// (Base speed * Run speed (if shift pressed) + Velocity acceleration * velocityY (faster if going up, slower if going down)) * deltaTime
		float velocity = (_camera->getBaseSpeed() * (1 + RUN_SPEED * shiftPressed) + VELOCITY_ACCELERATION * getVelocityY()) * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			finalLocation += velocity * forward;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			finalLocation += velocity * -right;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			finalLocation += velocity * -forward;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			finalLocation += velocity * right;
		if (_gamemode == SPECTATOR)
		{
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
				finalLocation += velocity * _camera->getAltitude();
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_MENU) == GLFW_PRESS)
				finalLocation += velocity * -_camera->getAltitude();
		}
		else
		{
			if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
					&& getBlockUnder().isSolid)
				setVelocityY(JUMP_STRENGTH);
		}

		// Camera movement
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		double mouseX, mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		const float rotX = _camera->getSensitivity() * static_cast<float>(mouseY - (static_cast<float>(_camera->getHeight()) / 2)) / static_cast<float>(_camera->getHeight());
		const float rotY = _camera->getSensitivity() * static_cast<float>(mouseX - (static_cast<float>(_camera->getWidth()) / 2)) / static_cast<float>(_camera->getWidth());

		_camera->setYaw(_camera->getYaw() + rotY);
    	_camera->setPitch(_camera->getPitch() - rotX);

		if (_camera->getPitch() > 89.99f)
			_camera->setPitch(89.99f);
		else if (_camera->getPitch() < -89.99f)
			_camera->setPitch(-89.99f);
		
		if (_camera->getYaw() < -179.99f)
			_camera->setYaw(180.0f);
		else if (_camera->getYaw() > 179.99f)
			_camera->setYaw(-180.0f);

		_camera->setOrientation(translateDirection(_camera->getYaw(), _camera->getPitch()));
	
		glfwSetCursorPos(window, (static_cast<float>(_camera->getWidth()) / 2), (static_cast<float>(_camera->getHeight()) / 2));
	}
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (getLocation() != finalLocation)
	{
		if ((_world->getBlockAt(finalLocation).isSolid || _world->getBlockAt(finalLocation.clone().add(0.0, 1.0, 0.0)).isSolid)
				&& _gamemode != SPECTATOR)
			return;
		teleport(finalLocation);
		checkFogChange();
	}
}

void Player::teleport(const Location & location)
{
	 _location = location;
	 _camera->setPosition(location.clone().add(0.0, CAMERA_OFFSET_Y, 0.0).clone().getVec3());
}

Block Player::getTargetedBlock() const
{
	float reach = 0;
	glm::vec3 orientation = _camera->getOrientation();
	glm::vec3 position = _camera->getPosition();
	BlockType hitBlock;

	do
	{
		hitBlock = _world->getBlockAt(position);
		if (hitBlock.isSolid)
			return (Block){ Location(position).blockalize(), hitBlock };
		position += glm::dot(orientation, glm::vec3(0.1));
		reach += 0.1;
	} while (reach <= 3 + (_gamemode == SPECTATOR) && hitBlock.type == AIR);
	return (Block){ Location(position).blockalize(), hitBlock };
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
	if (getBlockAtEyeLocation().type == LAVA)
	{
		_camera->setFogStart(0.005f);
		_camera->setFogEnd(0.015f);
	}
	else
	{
		_camera->setFogStart(FOG_START);
		_camera->setFogEnd(FOG_END);
	}
}

bool Player::isWithinRenderDistance(Chunk * chunk) const
{
	return !(chunk->getChunkX() > static_cast<int>(std::floor(getLocation().getX() / CHUNK_WIDTH)) + _camera->getRenderDistance()
	|| chunk->getChunkX() < static_cast<int>(std::floor(getLocation().getX() / CHUNK_WIDTH)) - _camera->getRenderDistance()
	|| chunk->getChunkZ() > static_cast<int>(std::floor(getLocation().getZ() / CHUNK_DEPTH)) + _camera->getRenderDistance()
	|| chunk->getChunkZ() < static_cast<int>(std::floor(getLocation().getZ() / CHUNK_DEPTH)) - _camera->getRenderDistance());
}
