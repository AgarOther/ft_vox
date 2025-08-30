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
		static uint32_t				getTint(Material material, BlockFace blockface);
	private:

		static BlockTypeMap 		_types;
		typedef std::unordered_map<std::pair<Material, BlockFace>, uint32_t, PairHash> TintMap;
		static TintMap				_tints;
};
