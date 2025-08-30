#pragma once

#include <cstdint>
#include <tuple>
#include <vector>
#include <string>
#include "Location.hpp"

enum ObjectType
{
	VERTICES,
	INDICES
};

enum Material
{
	UNKNOWN,
	GRASS_BLOCK,
	STONE,
	DIRT,
	BEDROCK,
	SAND,
	END_STONE,
	OAK_LOG,
	OAK_LEAVES,
	ACACIA_LOG,
	CARTOGRAPHY_TABLE,
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
	bool		isSolid;
	bool		isTransparent;
};

enum ModelType
{
	BLOCK,
	SKYBOX,
	CROSSHAIR
};

enum BlockFace
{
	FACE_FRONT,
	FACE_BACK,
	FACE_LEFT,
	FACE_RIGHT,
	FACE_TOP,
	FACE_BOTTOM
};

struct DebugInfo
{
	size_t	blocks;
	size_t	drawCalls;
	size_t	triangles;
	bool	wireframe;
};

enum Gamemode
{
	CREATIVE,
	SURVIVAL
};

struct Block
{
	Location	position;
	BlockType	blockType;
};

enum ChunkState
{
	IDLE,
	GENERATED,
	DIRTY,
	CLEANED,
	MESHED,
	UPLOADED
};

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

struct PairHash
{
	size_t operator()(const std::pair<int, int> & pair) const
	{
		size_t h1 = std::hash<int>{}(pair.first);
		size_t h2 = std::hash<int>{}(pair.second);
		return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
	}
};
