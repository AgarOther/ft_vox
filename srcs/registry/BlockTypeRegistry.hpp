#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include "glm/glm.hpp"

enum Material
{
	UNKNOWN,
	GRASS_BLOCK,
	STONE,
	DIRT,
	BEDROCK,
	SAND,
	GRAVEL,
	END_STONE,
	OAK_LOG,
	OAK_LEAVES,
	ACACIA_LOG,
	CARTOGRAPHY_TABLE,
	WATER,
	LAVA,
	NETHERRACK,
	GLOWSTONE,
	SOUL_SAND,
	CHERRY_LEAVES,
	CHERRY_LOG,
	OBSIDIAN,
	AIR = 255
};

struct BlockType
{
	Material	type;
	std::string	name;
	bool		isVisible;
	bool		isSolid;
	bool		isTransparent;
	bool		isLiquid;
};

enum BlockFace
{
	FACE_FRONT,
	FACE_BACK,
	FACE_LEFT,
	FACE_RIGHT,
	FACE_TOP,
	FACE_BOTTOM
};

struct PairHash
{
	size_t operator()(const std::pair<int, int> & pair) const
	{
		return std::hash<int>()(pair.first) ^ (std::hash<int>()(pair.second) << 1);
	}
};

class BlockTypeRegistry
{
	public:
		static void					init();
		static const BlockType &	getBlockType(uint8_t id);
		static const BlockType &	getBlockType(const std::string & name);
		typedef std::unordered_map<uint8_t, BlockType> BlockTypeMap;
		static const BlockTypeMap &	getBlockTypeMap();
	private:
		static BlockTypeMap 		_types;
};
