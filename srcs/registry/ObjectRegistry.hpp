#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <string>

enum ObjectType
{
	VERTICES,
	INDICES
};

struct Object
{
	std::vector<float>		vertices;
	std::vector<uint16_t>	indices;
};

enum ModelType
{
	BLOCK,
	SKYBOX,
	CROSSHAIR,
	BLOCK_OVERLAY
};

class ObjectRegistry
{
	public:
		ObjectRegistry() = delete;
		~ObjectRegistry() = delete;

		static void				init();
		static const Object &	getObject(uint8_t id);
		static Object			getObjectFromFile(const std::string & filepath);
	private:
		typedef std::unordered_map<uint8_t, Object> ObjectMap;
		static ObjectMap 		_objects;
};
