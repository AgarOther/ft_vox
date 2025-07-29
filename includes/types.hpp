#pragma once
#include <cstdint>
#include <vector>
#include <string>

typedef enum e_objectType
{
	VERTICES,
	INDICES
}			ObjectType;

typedef enum e_Material
{
	AIR,
	STONE
}			Material;

typedef struct s_Object
{
	std::vector<float>		vertices;
	std::vector<uint32_t>	indices;
}				Object;

typedef struct s_blockType
{
	uint8_t		id;
	std::string	name;
	Material	type;
	bool		isVisible;
}				BlockType;
