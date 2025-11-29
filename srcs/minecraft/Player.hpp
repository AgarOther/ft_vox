#pragma once

#include "Location.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "BoundingBox.hpp"
#include "types.hpp"
#include <cstdint>

#define JUMP_STRENGTH 7.7f
#define RUN_SPEED 0.75f
#define VELOCITY_ACCELERATION 0.15f
#define CAMERA_OFFSET_Y 1.5f

enum Gamemode
{
	SPECTATOR,
	SURVIVAL
};

struct Block
{
	Location	position;
	BlockType	blockType;
};

class Player
{
	public:
		Player() {}
		Player(const std::string & name, Camera * camera, World * world);
		~Player() {};

		const std::string &	getName() const { return _name; }
		uint8_t				getHealth() const { return _health; }
		Camera *			getCamera() const { return _camera; }
		const Location &	getLocation() const { return _location; }
		const Location &	getSpawnLocation() const { return _spawnLocation; }
		const BoundingBox &	getBoundingBox() const { return _boundingBox; }
		Gamemode			getGamemode() const { return _gamemode; }
		bool				isOnGround() const { return _velocity.y == 0; }
		glm::vec3			getVelocity() const { return _velocity; }
		float				getVelocityX() const { return _velocity.x; }
		float				getVelocityY() const { return _velocity.y; }
		float				getVelocityZ() const { return _velocity.z; }
		World *				getWorld() const { return _world; }
		Block				getTargetedBlock() const;
		BlockType			getBlockUnder(int xOffset = 0, int yOffset = 0, int zOffset = 0) const;
		BlockType			getBlockAtEyeLocation() const;

		void				setName(const std::string & name) { _name = name; }
		void				setHealth(uint8_t health) { _health = health; }
		void				teleport(const Location & location);
		void				setSpawnLocation(const Location & spawnLocation) { _spawnLocation = spawnLocation; }
		void				setBoundingBox(const BoundingBox & boundingBox) { _boundingBox = boundingBox; }
		void				setGamemode(Gamemode gamemode) { _gamemode = gamemode; }
		void				setVelocity(const glm::vec3 & velocity) { _velocity = velocity; }
		void				setVelocityX(float velocityX) { _velocity.x = velocityX; }
		void				setVelocityY(float velocityY) { _velocity.y = velocityY; }
		void				setVelocityZ(float velocityZ) { _velocity.z = velocityZ; }
		void				setWorld(World * world);

		void				interceptInputs(GLFWwindow * window, float deltaTime);
	private:
		std::string			_name;
		uint8_t				_health;
		Camera *			_camera;
		World *				_world;
		Location			_location;
		Location			_spawnLocation;
		BoundingBox			_boundingBox;
		Gamemode			_gamemode;
		glm::vec3			_velocity;
		bool				_spawned;
};
