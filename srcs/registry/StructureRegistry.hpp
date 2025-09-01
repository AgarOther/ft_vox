#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "BlockTypeRegistry.hpp"

enum StructureType
{
	TREE
};

typedef std::vector<std::pair<glm::vec3, Material>> StructureData;

struct Structure
{
	std::string			name;
	StructureData		data;
	glm::vec3			size;
};

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
