#include "InputManager.hpp"
#include "Scene.hpp"
#include "utils.hpp"

void InputManager::interceptKeyboard()
{

}

void InputManager::interceptOneTimeClicks(GLFWwindow * window, int button, int action, int mods)
{
	(void) mods;
	Scene * scene = reinterpret_cast<Scene *>(glfwGetWindowUserPointer(window));
	Player * player = scene->getPlayer();
	World * world = player->getWorld();

	if (action == GLFW_PRESS && !player->getCamera()->isLocked())
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

void InputManager::interceptMouse()
{
	
}
