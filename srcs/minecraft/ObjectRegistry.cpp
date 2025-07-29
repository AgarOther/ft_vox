#include <iostream>
#include "ObjectRegistry.hpp"
#include "utils.hpp"

ObjectRegistry::ObjectMap ObjectRegistry::_objects;

void ObjectRegistry::init()
{
	_objects[0] = {};
	_objects[1] = getObjectFromFile("assets/models/block.obj");
}

const Object & ObjectRegistry::getObject(uint8_t id)
{
	auto it = _objects.find(id);
	if (it != _objects.end())
		return it->second;
	std::cerr << "[ObjectRegistry] Warning: Requested unknown object ID " << id << ", returning default.\n";
	return _objects[0]; // air
}
