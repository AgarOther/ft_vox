#pragma once

#include "Location.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "BoundingBox.hpp"
#include <cstdint>

class Player
{
	public:
		Player(const std::string & name, int width, int height, World * world);
		~Player();

		const std::string &	getName() const { return _name; }
		uint8_t				getHealth() const { return _health; }
		Camera *			getCamera() { return _camera; }
		const Location &	getLocation() const { return _location; }
		const Location &	getSpawnLocation() const { return _spawnLocation; }
		const BoundingBox &	getBoundingBox() const { return _boundingBox; }

		void				setName(const std::string & name) { _name = name; }
		void				setHealth(uint8_t health) { _health = health; }
		void				teleport(const Location & location) { _location = location; }
		void				setSpawnLocation(const Location & spawnLocation) { _spawnLocation = spawnLocation; }
		void				setBoundingBox(const BoundingBox & boundingBox) { _boundingBox = boundingBox; }

		void				interceptInputs(GLFWwindow * window, float deltaTime);
	private:
		std::string			_name;
		uint8_t				_health;
		Camera *			_camera;
		World *				_world;
		Location 			_location;
		Location			_spawnLocation;
		BoundingBox			_boundingBox;
};
