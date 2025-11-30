#include <iostream>
#include <sstream>
#include "ObjectRegistry.hpp"
#include "utils.hpp"
#include "errors.hpp"

ObjectRegistry::ObjectMap ObjectRegistry::_objects;

Object ObjectRegistry::getObjectFromFile(const std::string & filepath)
{
	ObjectType type;
	Object object;

	type = VERTICES;
	const std::string file = getFileAsString(filepath.c_str());
	if (file.empty())
		handleExit(FAILURE_OBJECT);
	std::istringstream lineBuffer(file);
	for (std::string line; std::getline(lineBuffer, line);)
	{
		if ((!line.empty() && line[0] == '#') || line.empty() || (!line.empty() && line[0] == '\n'))
			continue;
		else if (!line.empty() && line[0] == 'I')
		{
			type = INDICES;
			continue;
		}
		std::vector<std::string> tmp = ft_split(line, ' ');
		for (std::string str : tmp)
		{
			if (!str.empty() && str[0] == '#')
				continue;
			if (type == VERTICES)
				object.vertices.push_back(std::atof(str.c_str()));
			else
				object.indices.push_back(std::atoi(str.c_str()));
		}
	}
	return object;
}

void ObjectRegistry::init()
{
	_objects[BLOCK] = getObjectFromFile("assets/model/block.obj");
	_objects[SKYBOX] = getObjectFromFile("assets/model/skybox.obj");
	_objects[CROSSHAIR] = getObjectFromFile("assets/model/crosshair.obj");
	_objects[BLOCK_OVERLAY] = getObjectFromFile("assets/model/block_overlay.obj");
	_objects[FILTER] = getObjectFromFile("assets/model/filter.obj");
}

const Object & ObjectRegistry::getObject(uint8_t id)
{
	auto it = _objects.find(id);
	if (it != _objects.end())
		return it->second;
	std::cerr << "[ObjectRegistry] Warning: Requested unknown object ID " << (int)id << ", returning default.\n";
	return _objects[0]; // block
}
