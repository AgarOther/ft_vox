#pragma once
#include <unordered_map>
#include <cstdint>
#include <string>
#include "types.hpp"

class StructureRegistry
{
	public:
		static void					init();
		static const Structure &	getStructure(uint8_t id);
		static const Structure &	getStructure(const std::string & name);
		typedef std::unordered_map<uint8_t, Structure> StructureMap;
		static const StructureMap &	getStructureMap();
	private:
		static StructureMap 		_structures;
};
