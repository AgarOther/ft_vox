#pragma once

#include <cstdint>
#include <vector>
#include <string>

enum ObjectType
{
	VERTICES,
	INDICES
};

enum Material
{
	UNKNOWN,
	STONE,
	AIR = 255
};

struct Object
{
	std::vector<float>		vertices;
	std::vector<uint32_t>	indices;
};

struct BlockType
{
	Material	type;
	std::string	name;
	bool		isVisible;
};

enum ModelType
{
	BLOCK = 1
};
