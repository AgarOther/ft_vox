#include <iostream>
#include "BlockTypeRegistry.hpp"

BlockTypeRegistry::BlockTypeMap BlockTypeRegistry::_types;

void BlockTypeRegistry::init()
{
	_types[0] = { UNKNOWN, "unknown", true };
	_types[1] = { STONE, "stone", true };
	_types[2] = { DIRT, "dirt", true };
	_types[3] = { BEDROCK, "bedrock", true };
	_types[255] = {  AIR, "air",false };
}

const BlockType & BlockTypeRegistry::getBlockType(uint8_t id)
{
	auto it = _types.find(id);
	if (it != _types.end())
		return it->second;
	std::cerr << "[ObjectRegistry] Warning: Requested unknown object ID " << (int)id << ", returning default.\n";
	return _types[0]; // air
}

const BlockType & BlockTypeRegistry::getBlockType(const std::string & name)
{
	for (const auto & pair : _types)
	{
		if (pair.second.name == name)
			return pair.second;
	}
	std::cerr << "[ObjectRegistry] Warning: Requested unknown object named " << name << ", returning default.\n";
	return _types[0];
}
