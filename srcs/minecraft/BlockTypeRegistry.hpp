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
	private:
		typedef std::unordered_map<uint8_t, BlockType> BlockTypeMap;
		static BlockTypeMap 		_types;
};
