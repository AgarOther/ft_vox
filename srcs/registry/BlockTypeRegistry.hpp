#pragma once

#include <cstdint>
#include <unordered_map>
#include "types.hpp"

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
