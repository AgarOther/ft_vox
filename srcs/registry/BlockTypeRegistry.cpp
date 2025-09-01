#include <iostream>
#include "BlockTypeRegistry.hpp"

BlockTypeRegistry::BlockTypeMap BlockTypeRegistry::_types;
BlockTypeRegistry::TintMap BlockTypeRegistry::_tints;

void BlockTypeRegistry::initTints()
{
	if (!_tints.empty())
		return;
}

void BlockTypeRegistry::init()
{
	if (!_types.empty())
		return;

	_types[UNKNOWN]				= { UNKNOWN, "unknown", true, true, false, false };
	_types[STONE]				= { STONE, "stone", true, true, false, false };
	_types[DIRT]				= { DIRT, "dirt", true, true, false, false };
	_types[BEDROCK]				= { BEDROCK, "bedrock", true, true, false, false };
	_types[SAND]				= { SAND, "sand", true, true, false, false };
	_types[GRAVEL]				= { GRAVEL, "gravel", true, true, false, false };
	_types[END_STONE]			= { END_STONE, "end_stone", true, true, false, false };
	_types[OAK_LOG]				= { OAK_LOG, "oak_log", true, true, false, false };
	_types[OAK_LEAVES]			= { OAK_LEAVES, "oak_leaves", true, true, true, false };
	_types[ACACIA_LOG]			= { ACACIA_LOG, "acacia_log", true, true, false, false };
	_types[CARTOGRAPHY_TABLE]	= { CARTOGRAPHY_TABLE, "cartography_table", true, true, false, false };
	_types[GRASS_BLOCK]			= { GRASS_BLOCK, "grass_block", true, true, false, false };
	_types[WATER]				= { WATER, "water", true, false, false, true };
	_types[LAVA]				= { LAVA, "lava", true, false, false, true };
	_types[NETHERRACK]			= { NETHERRACK, "netherrack", true, true, false, false };
	_types[GLOWSTONE]			= { GLOWSTONE, "glowstone", true, true, false, false };
	_types[SOUL_SAND]			= { SOUL_SAND, "soul_sand", true, true, false, false };
	_types[AIR]					= { AIR, "air",false, false, false, false };

	initTints();
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

glm::u8vec3 BlockTypeRegistry::getTint(Material material, BlockFace blockface)
{
	auto it = _tints.find({material, blockface});
	if (it != _tints.end())
		return it->second;
	return {255, 255, 255}; // normal (uint max)
}

const BlockTypeRegistry::BlockTypeMap & BlockTypeRegistry::getBlockTypeMap()
{
	return _types;
}
