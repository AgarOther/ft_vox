#include "World.hpp"
#include <glm/glm.hpp>
#include "types.hpp"
#include "utils.hpp"
#include "BlockTypeRegistry.hpp"
#include <Chunk.hpp>
#include <cmath>
#include <cstdlib>
#include <unordered_map>

Player::Player(const std::string & name, int width, int height, World * world)
{
	_name = name;
	_health = 20;
	_world = world;
	_spawnLocation = Location(0.0, 0.0, 0.0);
	_camera = new Camera(width, height, _spawnLocation.clone().add(0.0, CAMERA_OFFSET_Y, 0.0).getVec3());
	_location = _spawnLocation.clone();
	_boundingBox = BoundingBox(Location(0, 0, 0), Location(1, 2, 1));
	_gamemode = CREATIVE;
	_velocity = glm::vec3(0.0f);
	_spawned = false;

	_world->addPlayer(this);
	_world->load();
}

Player::~Player()
{
	if (_camera)
		delete _camera;
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	static bool lastFramePressedF3 = false;
	const bool keyPressedF3 = glfwGetKey(window, GLFW_KEY_F3);
	if (keyPressedF3 && !lastFramePressedF3)
		_camera->setGui(!_camera->hasGuiOn());

	static bool lastFramePressedF11 = false;
	const bool keyPressedF11 = glfwGetKey(window, GLFW_KEY_F11);
	if (keyPressedF11 && !lastFramePressedF11)
		toggleFullscreen(window, _camera);

	static bool lastFrameKeysLocked = false;
	const bool keyPressedL = glfwGetKey(window, GLFW_KEY_L);
	if (keyPressedL && !lastFrameKeysLocked)
		_camera->setLocked(!_camera->isLocked());

	lastFramePressedF3 = keyPressedF3;
	lastFramePressedF11 = keyPressedF11;
	lastFrameKeysLocked = keyPressedL;

	if (!_spawned)
	{
		if (_world->getChunkAt(0, 0)->getState() >= GENERATED)
		{
			_spawnLocation.setY(_world->getHighestYAtChunkLocation(0, 0));
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

	if (!_camera->isLocked())
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
		if (_gamemode == CREATIVE)
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
		Location test = finalLocation.clone();
		test.setX(round(test.getX()));
		test.setZ(round(test.getZ()));
		if (_world->getBlockAt(test).isSolid && _gamemode != CREATIVE)
			return;
		teleport(finalLocation);
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
	} while (reach <= 3 + (_gamemode == CREATIVE) && hitBlock.type == AIR);
	return (Block){ Location(position).blockalize(), hitBlock };
}

BlockType Player::getBlockUnder(int xOffset, int yOffset, int zOffset) const
{
	if (getLocation().getY() >= CHUNK_HEIGHT || getLocation().getY() < 0)
		return BlockTypeRegistry::getBlockType(AIR);
	Location blockLocation = getLocation().clone();
	blockLocation.setX(round(blockLocation.getX() - xOffset));
	blockLocation.setY(ceil(blockLocation.getY() - yOffset - 1));
	blockLocation.setZ(round(blockLocation.getZ() - zOffset));
	return _world->getBlockAt(blockLocation);
}

void Player::setWorld(World * world)
{
	_world->removePlayer(this);
	_world->shutdown();

	world->load();
	world->addPlayer(this);

	_world = world;
}
