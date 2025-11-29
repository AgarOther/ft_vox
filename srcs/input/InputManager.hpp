#pragma once

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

class InputManager
{
	public:
		InputManager() = delete;
		~InputManager() = delete;

		static void interceptKeyboard();
		static void interceptOneTimeKeys(GLFWwindow * window, int key, int scancode, int action, int mods);
		static void interceptMouse();
};
