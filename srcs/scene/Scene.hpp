#pragma once

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.hpp"
#include "Player.hpp"
#include "WorldManager.hpp"

class Scene
{
	public:
		Scene() {}
		~Scene();

		void				init();

		GLFWwindow *		getWindow() const { return _window; }
		Camera *			getCamera() const { return _camera; }
		Player *			getPlayer() const { return _player; }
		WorldManager *		getWorldManager() const { return _worldManager; }
		int					getWidth() const { return _width; }
		int					getHeight() const { return _height; }
		int					getFPSGoal() const { return _fpsGoal; }

		void				setFPSGoal(int fpsGoal) { _fpsGoal = fpsGoal; }

	private:
		GLFWwindow *		_window;
		Camera *			_camera;
		Player *			_player;
		WorldManager *		_worldManager;
		int					_width;
		int					_height;
		int					_fpsGoal;
};
