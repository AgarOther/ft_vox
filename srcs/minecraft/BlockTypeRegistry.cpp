#include <iostream>
#include "BlockTypeRegistry.hpp"

BlockTypeRegistry::BlockTypeMap BlockTypeRegistry::_types;

void BlockTypeRegistry::init()
{
	_types[UNKNOWN]				= { UNKNOWN, "unknown", true, true, false };
	_types[STONE]				= { STONE, "stone", true, true, false };
	_types[DIRT]				= { DIRT, "dirt", true, true, false };
	_types[BEDROCK]				= { BEDROCK, "bedrock", true, true, false };
	_types[SAND]				= { SAND, "sand", true, true, false };
	_types[END_STONE]			= { END_STONE, "end_stone", true, true, false };
	_types[OAK_LOG]				= { OAK_LOG, "oak_log", true, true, false };
	_types[OAK_LEAVES]			= { OAK_LEAVES, "oak_leaves", true, true, true };
	_types[ACACIA_LOG]			= { ACACIA_LOG, "acacia_log", true, true, false };
	_types[CARTOGRAPHY_TABLE]	= { CARTOGRAPHY_TABLE, "cartography_table", true, true, false };
	_types[GRASS_BLOCK]			= { GRASS_BLOCK, "grass_block", true, true, false };
	_types[AIR]					= {  AIR, "air",false, false, true };
}

const BlockType & BlockTypeRegistry::getBlockType(uint8_t id)
{
	auto it = _types.find(id);
	if (it != _types.end())
		return it->second;
	std::cerr << "[BlockTypeRegistry] Warning: Requested unknown block ID " << (int)id << ", returning default.\n";
	return _types[0]; // air
}

const BlockType & BlockTypeRegistry::getBlockType(const std::string & name)
{
	for (const auto & pair : _types)
	{
		if (pair.second.name == name)
			return pair.second;
	}
	std::cerr << "[BlockTypeRegistry] Warning: Requested unknown block named " << name << ", returning default.\n";
	return _types[0];
}

const BlockTypeRegistry::BlockTypeMap & BlockTypeRegistry::getBlockTypeMap()
{
	return _types;
}
