#include "World.hpp"
#include "WorldManager.hpp"
#include "utils.hpp"

WorldManager::WorldMap WorldManager::_worldMap;

World * WorldManager::createWorld(const std::string & worldName, Environment environment, std::optional<Seed> seed)
{
	World * world = new World(worldName, seed.value(), environment);
	if (!seed)
		seed = generateRandomSeed();
	if (_worldMap.find(worldName) == _worldMap.end())
		_worldMap.try_emplace(worldName, std::unique_ptr<World>(world));
	return world;
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
