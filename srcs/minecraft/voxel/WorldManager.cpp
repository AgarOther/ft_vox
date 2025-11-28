#include "World.hpp"
#include "WorldManager.hpp"
#include "utils.hpp"

WorldManager::WorldMap WorldManager::_worldMap;

void WorldManager::createWorld(const std::string & worldName, Environment environment, std::optional<Seed> seed)
{
	if (!seed)
		seed = generateRandomSeed();
	_worldMap.try_emplace(worldName, std::make_unique<World>(worldName, seed.value(), environment));
}

World * WorldManager::getWorld(const std::string & name)
{
	WorldMap::iterator it = _worldMap.find(name);

	if (it != _worldMap.end())
		return it->second.get();
	return nullptr;
}

void WorldManager::deleteWorld(const std::string & worldName)
{
	_worldMap.erase(worldName);
}
