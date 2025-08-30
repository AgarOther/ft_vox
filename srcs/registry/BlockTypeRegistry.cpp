#include <iostream>
#include "BlockTypeRegistry.hpp"

BlockTypeRegistry::BlockTypeMap BlockTypeRegistry::_types;
BlockTypeRegistry::TintMap BlockTypeRegistry::_tints;

void BlockTypeRegistry::initTints()
{
	if (!_tints.empty())
		return;

	_tints[{GRASS_BLOCK, FACE_TOP}]	= 140 << 24 | 204 << 16 | 89 << 8 | 255;

	_tints[{OAK_LEAVES, FACE_FRONT}]	= 140 << 24 | 204 << 16 | 89 << 8 | 255;
	_tints[{OAK_LEAVES, FACE_BACK}]	= 140 << 24 | 204 << 16 | 89 << 8 | 255;
	_tints[{OAK_LEAVES, FACE_LEFT}]	= 140 << 24 | 204 << 16 | 89 << 8 | 255;
	_tints[{OAK_LEAVES, FACE_RIGHT}]	= 140 << 24 | 204 << 16 | 89 << 8 | 255;
	_tints[{OAK_LEAVES, FACE_TOP}]	= 140 << 24 | 204 << 16 | 89 << 8 | 255;
	_tints[{OAK_LEAVES, FACE_BOTTOM}]	= 140 << 24 | 204 << 16 | 89 << 8 | 255;
}

void BlockTypeRegistry::init()
{
	if (!_types.empty())
		return;

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

uint32_t BlockTypeRegistry::getTint(Material material, BlockFace blockface)
{
	auto it = _tints.find({material, blockface});
	if (it != _tints.end())
		return it->second;
	return 0xFFFFFFFF; // normal (uint max)
}

const BlockTypeRegistry::BlockTypeMap & BlockTypeRegistry::getBlockTypeMap()
{
	return _types;
}
