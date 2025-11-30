#include "InputManager.hpp"
#include "Scene.hpp"
#include "utils.hpp"

void InputManager::interceptKeyboard(Scene * scene, float deltaTime)
{
	Player * player = scene->getPlayer();
	Camera * camera = scene->getCamera();
	World * world = player->getWorld();
	GLFWwindow * window = scene->getWindow();
	Gamemode gamemode = player->getGamemode();
	Location finalLocation = player->getLocation();
	glm::vec3 forward = camera->computeForward();
	const glm::vec3 right = glm::normalize(glm::cross(forward, camera->getAltitude())); // Right direction is based on forward and altitude (up vector)
	Material playerStandingBlock = player->getBlockUnder(0, -1, 0).material;
	const float liquidModifier = player->getGamemode() == SURVIVAL ? (playerStandingBlock == WATER ? 0.4f : playerStandingBlock == LAVA ? 0.1f : 1.0f) : 1.0f;

	if (!camera->isLocked() && glfwGetWindowAttrib(window, GLFW_FOCUSED) && player->hasSpawned())
	{
		// Key management
		bool shiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

		// (Base speed * Run speed (if shift pressed) + Velocity acceleration * velocityY (faster if going up, slower if going down))
		// * liquidModifier (is in water? lava?) * deltaTime
		float velocity = (camera->getBaseSpeed() * (1 + RUN_SPEED * shiftPressed) + VELOCITY_ACCELERATION * player->getVelocityY())
							* liquidModifier * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			finalLocation += velocity * forward;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			finalLocation += velocity * -right;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			finalLocation += velocity * -forward;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			finalLocation += velocity * right;
		if (gamemode == SPECTATOR)
		{
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
				finalLocation += velocity * camera->getAltitude();
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_MENU) == GLFW_PRESS)
				finalLocation += velocity * -camera->getAltitude();
		}
		else
		{
			if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
					&& player->getBlockUnder().isSolid)
				player->setVelocityY(JUMP_STRENGTH);
		}
	}

	if (player->getLocation() != finalLocation)
	{
		if ((world->getBlockAt(finalLocation).isSolid || world->getBlockAt(finalLocation.clone().add(0.0, 1.0, 0.0)).isSolid)
				&& gamemode != SPECTATOR)
			return;
		player->teleport(finalLocation);
		player->checkFogChange();
	}
}

void InputManager::interceptOneTimeClicks(GLFWwindow * window, int button, int action, int mods)
{
	(void) mods;
	Scene * scene = reinterpret_cast<Scene *>(glfwGetWindowUserPointer(window));
	Player * player = scene->getPlayer();
	World * world = player->getWorld();

	if (action == GLFW_PRESS && !player->getCamera()->isLocked() && player->hasSpawned())
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			Block block = player->getTargetedBlock();
			if (block.blockType.material != AIR)
			{
				const Location position = player->getTargetedBlock().position;
				Chunk * chunk = world->getChunkAt(position.getX(), position.getZ());
				if (chunk)
					chunk->changeBlockAt(player->getTargetedBlock().position, AIR);
			}
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			Block block = player->getTargetedBlock();
			if (!block.blockType.isSolid)
				return;
			Location newBlockLocation = block.position.clone().add(0.0, 1.0, 0.0);
			Chunk * chunk = world->getChunkAt(block.position.getX(), block.position.getZ());
			if (!world->getBlockAt(newBlockLocation).isSolid)
				chunk->changeBlockAt(newBlockLocation, player->getBlockInHand());
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			Block block = player->getTargetedBlock();
			if (!block.blockType.isSolid)
				return;
			player->setBlockInHand(block.blockType.material);
		}
	}
}
void InputManager::interceptOneTimeKeys(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	(void) scancode;
	(void) mods;

	Scene * scene = reinterpret_cast<Scene *>(glfwGetWindowUserPointer(window));
	Camera * camera = scene->getCamera();

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_F3)
			camera->setGui(!camera->hasGuiOn());
		if (key == GLFW_KEY_F11)
			toggleFullscreen(window, camera);
		if (key == GLFW_KEY_L)
			camera->setLocked(!camera->isLocked());
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, true);
	}
}

void InputManager::interceptScroll(GLFWwindow * window, double xoffset, double yoffset)
{
	(void) xoffset;
	Scene * scene = reinterpret_cast<Scene *>(glfwGetWindowUserPointer(window));
	Player * player = scene->getPlayer();
	int newBlock = player->getBlockInHand() + (yoffset > 0 ? 1 : -1);

	if (newBlock >= AIR)
		newBlock = UNKNOWN;
	else if (newBlock < UNKNOWN)
		newBlock = AIR - 1;
	player->setBlockInHand(static_cast<Material>(newBlock));
}

static glm::vec3 translateDirection(const float yaw, const float pitch)
{
	glm::vec3 direction;
	direction.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	direction.y = sinf(glm::radians(pitch));
	direction.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	return (direction);
}

void InputManager::interceptMouse(Scene * scene)
{
	GLFWwindow * window = scene->getWindow();
	Camera * camera = scene->getCamera();

	if (camera->isLocked() || !glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	double mouseX, mouseY;

	glfwGetCursorPos(window, &mouseX, &mouseY);

	const float rotX = camera->getSensitivity() * static_cast<float>(mouseY - (static_cast<float>(camera->getHeight()) / 2)) / static_cast<float>(camera->getHeight());
	const float rotY = camera->getSensitivity() * static_cast<float>(mouseX - (static_cast<float>(camera->getWidth()) / 2)) / static_cast<float>(camera->getWidth());

	camera->setYaw(camera->getYaw() + rotY);
	camera->setPitch(camera->getPitch() - rotX);

	if (camera->getPitch() > 89.99f)
		camera->setPitch(89.99f);
	else if (camera->getPitch() < -89.99f)
		camera->setPitch(-89.99f);
	
	float yaw = camera->getYaw();
	if (yaw > 180.0f)
		yaw -= 360.0f;
	else if (yaw < -180.0f)
		yaw += 360.0f;
	camera->setYaw(yaw);

	camera->setOrientation(translateDirection(camera->getYaw(), camera->getPitch()));

	glfwSetCursorPos(window, (static_cast<float>(camera->getWidth()) / 2), (static_cast<float>(camera->getHeight()) / 2));
}
