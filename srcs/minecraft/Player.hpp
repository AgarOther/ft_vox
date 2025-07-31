#pragma once

#include "Location.hpp"
#include "Camera.hpp"
#include <cstdint>

class Player
{
	public:
		Player(const std::string & name, const Camera & camera) :
			_name(name), _health(20), _camera(camera), _location(camera.getPosition()), _spawnLocation(camera.getPosition()) {}

		const std::string &	getName() const { return _name; }
		uint8_t				getHealth() const { return _health; }
		const Location &	getLocation() const { return _location; }
		const Location &	getSpawnLocation() const { return _spawnLocation; }

		void				setName(const std::string & name) { _name = name; }
		void				setHealth(uint8_t health) { _health = health; }
		void				teleport(const Location & location) { _location = location; }
		void				setSpawnLocation(const Location & spawnLocation) { _spawnLocation = spawnLocation; }
	private:
		std::string			_name;
		uint8_t				_health;
		Camera				_camera;
		Location 			_location;
		Location			_spawnLocation;
};
