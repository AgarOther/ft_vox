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
		size_t h1 = std::hash<int>{}(pair.first);
		size_t h2 = std::hash<int>{}(pair.second);
		return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
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
		static void					initTints();
		static glm::u8vec3				getTint(Material material, BlockFace blockface);
	private:

		static BlockTypeMap 		_types;
		typedef std::unordered_map<std::pair<Material, BlockFace>, glm::u8vec3, PairHash> TintMap;
		static TintMap				_tints;
};
