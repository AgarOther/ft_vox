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
	StructureData data;

	std::istringstream lineBuffer(path);
	for (std::string line; std::getline(lineBuffer, line);)
	{
		if ((!line.empty() && line[0] == '#') || line.empty() || (!line.empty() && line[0] == '\n'))
			continue;
		std::vector<std::string> tmp = ft_split(line, ',');
		if (tmp.empty())
		{
			continue;
		}
		std::pair<std::tuple<int, int, int>, Material> block;
		for (std::string str : tmp)
		{
			if (str.size() != 4)
				handleExit(FAILURE_STRUCTURE_DATA);
			std::vector<std::string> values = ft_split(str, ':');
			if (values.size() != 2)
				handleExit(FAILURE_STRUCTURE_DATA);
		}
	}
}

void StructureRegistry::init()
{
	_structures[0] = { TREE, "tree", getDataFromFile("tree.struct") };
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
