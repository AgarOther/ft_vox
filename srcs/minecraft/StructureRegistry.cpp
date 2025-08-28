#include <cctype>
#include <iostream>
#include "StructureRegistry.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include <sstream>

StructureRegistry::StructureMap StructureRegistry::_structures;

static StructureData getDataFromFile(const std::string & filename)
{
	const std::string path = "assets/structures/" + filename;
	const std::string file = getFileAsString(path.c_str());
	StructureData data;

	if (file.empty())
		handleExit(FAILURE_STRUCTURE);
	std::istringstream lineBuffer(file);
	for (std::string line; std::getline(lineBuffer, line);)
	{
		if ((!line.empty() && line[0] == '#') || line.empty() || (!line.empty() && line[0] == '\n'))
			continue;
		std::vector<std::string> tmp = ft_split(line, ',');
		if (tmp.empty())
			continue;
		else if (tmp.size() != 4 || tmp[0].length() < 3 || tmp[1].length() < 3 || tmp[2].length() < 3 || tmp[3].length() < 6)
		{
			std::cerr << "Faulty line: " << line << ". ";
			handleExit(FAILURE_STRUCTURE_DATA);
		}
		std::pair<glm::vec3, Material> block;
		std::string type = &tmp[3][5];
		block.first = { std::atoi(&tmp[0][2]), std::atoi(&tmp[1][2]), std::atoi(&tmp[2][2]) };
		block.second = getMaterialFromString(type);
		data.push_back(block);
	}
	return data;
}

void StructureRegistry::init()
{
	_structures[TREE] = { "tree", getDataFromFile("tree.struct"), glm::vec3(0) };

	for (auto it = _structures.begin(); it != _structures.end(); ++it)
	{
		int biggestX = 0;
		int biggestY = 0;
		int biggestZ = 0;
		Structure structure = it->second;
		for (std::pair<glm::vec3, Material> block : structure.data)
		{
			if (block.first.x > biggestX)
				biggestX = block.first.x;
			if (block.first.y > biggestY)
				biggestY = block.first.y;
			if (block.first.z > biggestZ)
				biggestZ = block.first.z;
		}
		it->second.size = glm::vec3(biggestX, biggestY, biggestZ);
	}
}

const Structure & StructureRegistry::getStructure(uint8_t id)
{
	auto it = _structures.find(id);
	if (it != _structures.end())
		return it->second;
	std::cerr << "[StructureRegistry] Warning: Requested unknown structure ID " << (int)id << ", returning default.\n";
	return _structures[0]; // tree
}

const Structure & StructureRegistry::getStructure(const std::string & name)
{
	for (const auto & pair : _structures)
	{
		if (pair.second.name == name)
			return pair.second;
	}
	std::cerr << "[StructureRegistry] Warning: Requested unknown structure named " << name << ", returning default.\n";
	return _structures[0];
}

const StructureRegistry::StructureMap & StructureRegistry::getStructureMap()
{
	return _structures;
}
