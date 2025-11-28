#pragma once

#include <cstdint>
#include <tuple>
#include <vector>
#include <string>
#include "Location.hpp"
#include "BlockTypeRegistry.hpp"

struct DebugInfo
{
	size_t	blocks;
	size_t	drawCalls;
	size_t	triangles;
	bool	wireframe;
};

enum Environment
{
	OVERWORLD,
	NETHER,
	THE_END
};

typedef unsigned long Seed;
