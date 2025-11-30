#pragma once

#include "Scene.hpp"

class InputManager
{
	public:
		InputManager() = delete;
		~InputManager() = delete;

		static void interceptKeyboard(Scene * scene, float deltaTime);
		static void interceptOneTimeKeys(GLFWwindow * window, int key, int scancode, int action, int mods);
		static void	interceptOneTimeClicks(GLFWwindow * window, int button, int action, int mods);
		static void interceptMouse(Scene * scene);
		static void	interceptScroll(GLFWwindow * window, double xoffset, double yoffset);
};
