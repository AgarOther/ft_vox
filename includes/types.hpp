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
