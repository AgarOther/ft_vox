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
	DIRT,
	BEDROCK,
	SAND,
	END_STONE,
	ACACIA_LOG,
	CARTOGRAPHY_TABLE,
	GRASS_BLOCK,
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
	void		setType(Material newType) { type = newType; }
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
