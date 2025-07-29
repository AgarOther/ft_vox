#include <iostream>
#include "BlockTypeRegistry.hpp"

BlockTypeRegistry::BlockTypeMap BlockTypeRegistry::_types;

void BlockTypeRegistry::init()
{
	_types[0] = { 0, "air", AIR, false };
	_types[1] = { 1, "stone", STONE, true };
}

const BlockType & BlockTypeRegistry::getBlockType(uint8_t id)
{
	auto it = _types.find(id);
	if (it != _types.end())
		return it->second;
	std::cerr << "[ObjectRegistry] Warning: Requested unknown object ID " << id << ", returning default.\n";
	return _types[0]; // air
}
