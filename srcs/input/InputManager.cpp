#include "InputManager.hpp"
#include "Scene.hpp"
#include "utils.hpp"

void InputManager::interceptKeyboard()
{

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
	}
}

void InputManager::interceptMouse()
{
	
}
