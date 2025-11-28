#pragma once

#include "types.hpp"
#include <memory>
#include <optional>
#include <unordered_map>

class World;

class WorldManager
{
	public:
		WorldManager() = delete;

		static void		createWorld(const std::string & worldName, Environment environment, std::optional<Seed> seed = std::nullopt);
		static World *	getWorld(const std::string & worldName);
		static void		deleteWorld(const std::string & worldName);

	private:
		typedef std::unordered_map<std::string, std::unique_ptr<World>> WorldMap;
		static WorldMap	_worldMap;
};
