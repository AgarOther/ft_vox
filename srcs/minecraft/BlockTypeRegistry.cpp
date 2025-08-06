#include <iostream>
#include "BlockTypeRegistry.hpp"

BlockTypeRegistry::BlockTypeMap BlockTypeRegistry::_types;

void BlockTypeRegistry::init()
{
	_types[0]	= { UNKNOWN, "unknown", true, true };
	_types[1]	= { STONE, "stone", true, true };
	_types[2]	= { DIRT, "dirt", true, true };
	_types[3]	= { BEDROCK, "bedrock", true, true };
	_types[4]	= { SAND, "sand", true, true };
	_types[5]	= { END_STONE, "end_stone", true, true };
	_types[6]	= { ACACIA_LOG, "acacia_log", true, true };
	_types[7]	= { CARTOGRAPHY_TABLE, "cartography_table", true, true };
	_types[8]	= { GRASS_BLOCK, "grass_block", true, true };
	_types[255]	= {  AIR, "air",false, false };
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

const BlockTypeRegistry::BlockTypeMap & BlockTypeRegistry::getBlockTypeMap()
{
	return _types;
}
