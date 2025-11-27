#include <cstdlib>
#include <mutex>
#include <vector>
#include <iostream>
#include "Chunk.hpp"
#include "ObjectRegistry.hpp"
#include "BlockTypeRegistry.hpp"
#include "StructureRegistry.hpp"
#include "Shader.hpp"
#include "World.hpp"
#include "errors.hpp"
#include "utils.hpp"

// If chunk does weird stuff, revert indices back to uint32_t

std::mutex g_debugMutex;

void Chunk::_generateStructures()
{
	if (rand() % 3)
		return;
	const Structure & tree = StructureRegistry::getStructure(TREE);
	const int x = rand() % 16;
	const int z = rand() % 16;
	int y = SEA_LEVEL;
	if (x + tree.size.x >= CHUNK_WIDTH || y + tree.size.y >= CHUNK_HEIGHT || z + tree.size.z >= CHUNK_DEPTH)
		return;
	while (_blocks[x][y][z] != AIR)
	{
		if (_blocks[x][y][z] == WATER)
			return;
		y++;
	}

	for (std::pair<glm::vec3, Material> data : tree.data)
	{
		const int dx = x + data.first.x;
		const int dy = y + data.first.y;
		const int dz = z + data.first.z;
		if (dx >= CHUNK_WIDTH || dy >= CHUNK_HEIGHT || dz >= CHUNK_DEPTH)
			continue;
		_blocks[dx][dy][dz] = data.second;
	}
}

void Chunk::_generateSand(Environment environment)
{
	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				if (environment == OVERWORLD)
				{
					if (_blocks[x][y][z] == WATER)
					{
						if (x + 1 < CHUNK_WIDTH && _blocks[x + 1][y][z] != WATER)
							_blocks[x + 1][y][z] = SAND;
						if (x - 1 >= 0 && _blocks[x - 1][y][z] != WATER)
							_blocks[x - 1][y][z] = SAND;
						if (z + 1 < CHUNK_DEPTH && _blocks[x][y][z + 1] != WATER)
							_blocks[x][y][z + 1] = SAND;
						if (z - 1 >= 0 && _blocks[x][y][z - 1] != WATER)
							_blocks[x][y][z - 1] = SAND;
						if (y - 1 >= 0 && _blocks[x][y - 1][z] != WATER)
							_blocks[x][y - 1][z] = GRAVEL;
					}
					else if (y + 1 < CHUNK_HEIGHT && _blocks[x][y + 1][z] == WATER)
						_blocks[x][y][z] = GRAVEL;
				}
				else if (environment == NETHER)
				{
					if (_blocks[x][y][z] == LAVA)
					{
						if (x + 1 < CHUNK_WIDTH && _blocks[x + 1][y][z] != LAVA)
							_blocks[x + 1][y][z] = SOUL_SAND;
						if (x - 1 >= 0 && _blocks[x - 1][y][z] != LAVA)
							_blocks[x - 1][y][z] = SOUL_SAND;
						if (z + 1 < CHUNK_DEPTH && _blocks[x][y][z + 1] != LAVA)
							_blocks[x][y][z + 1] = SOUL_SAND;
						if (z - 1 >= 0 && _blocks[x][y][z - 1] != LAVA)
							_blocks[x][y][z - 1] = SOUL_SAND;
						if (y - 1 >= 0 && _blocks[x][y - 1][z] != LAVA)
							_blocks[x][y - 1][z] = SOUL_SAND;
					}
				}
			}
		}
	}
}

void Chunk::generateBlocks(Environment environment)
{
	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				float worldX = static_cast<float>(_chunkX * CHUNK_WIDTH + x);
				float worldZ = static_cast<float>(_chunkZ * CHUNK_DEPTH + z);

				if (environment == OVERWORLD)
				{
					// I added arbitrary values to X/Z so that the noise doesn't make a weird symmetry at 0, 0
					const double noiseValue = (_world->getNoise().getNoise(worldX + 4242.42, worldZ + 2424.24, OCTAVES) + 1.0) * 0.5;
					const int height = static_cast<int>(noiseValue * SEA_LEVEL * 2);
					const int stoneOffset = static_cast<int>(floor(height / noiseValue)) % 3 + 3;
					if (y == height && y >= SEA_LEVEL)
						_blocks[x][y][z] = GRASS_BLOCK;
					else if (y == 0)
						_blocks[x][y][z] = BEDROCK;
					else if (y < height && y > height - stoneOffset)
						_blocks[x][y][z] = DIRT;
					else if (y <= height - stoneOffset)
						_blocks[x][y][z] = STONE;
					else if (y > height)
						_blocks[x][y][z] = y <= SEA_LEVEL ? WATER : AIR;
					else
						_blocks[x][y][z] = DIRT;
				}
				else if (environment == NETHER)
				{
					const double noiseValue = (_world->getNoise().getNoise(worldX + 4242.42, worldZ + 2424.24, OCTAVES) + 1.0) * 0.5;
					const int height = static_cast<int>(noiseValue * LAVA_LEVEL * 2);
					if (y == 0)
						_blocks[x][y][z] = BEDROCK;
					else if (y == CHUNK_HEIGHT - 1)
						_blocks[x][y][z] = BEDROCK;
					else if (y > height)
						_blocks[x][y][z] = y <= LAVA_LEVEL ? LAVA : AIR;
					else
						_blocks[x][y][z] = NETHERRACK;
				}
				else if (environment == THE_END)
				{
					const int islandCenterX = 0;
					const int islandCenterZ = 0;

					const int posX = _chunkX * CHUNK_WIDTH;
					const int posZ = _chunkZ * CHUNK_DEPTH;

					int worldX = posX + x - islandCenterX;
					int worldZ = posZ + z - islandCenterZ;
					int worldY = y - 16;
					double dist = sqrt(worldX * worldX + worldZ * worldZ + worldY * worldY);

					bool placed = false; // <--- NEW FLAG

					// === Main Chaos Island ===
					const double size = 80.0;
					double xd = static_cast<double>(worldX) / size;
					double yd = static_cast<double>(y) / 32.0;
					double zd = static_cast<double>(worldZ) / size;

					double centerFalloff = 1.0 / (dist * 0.05);
					if (centerFalloff < 0.0) centerFalloff = 0.0;

					double plateauFalloff = 0.0;
					if (yd < 0.4) plateauFalloff = yd * 2.5;
					else if (yd <= 0.6) plateauFalloff = 1.0;
					else if (yd < 1.0) plateauFalloff = 1.0 - (yd - 0.6) * 2.5;

					if (plateauFalloff > 0.0 && centerFalloff > 0.0)
					{
						double heightMapFalloff = 0.0;
						for (int octave = 1; octave < 5; ++octave)
						{
							double n = _world->getNoise().getNoise(
								xd * octave + islandCenterX,
								zd * octave + islandCenterZ,
								octave
							);
							n = (n + 1.0) * 0.5;
							heightMapFalloff += n * 0.01 * (octave * 10.0 - dist * 0.001);
						}
						heightMapFalloff += (0.5 - fabs(yd - 0.5)) * 0.15;

						if (heightMapFalloff > 0.0)
						{
							double density = centerFalloff * plateauFalloff * heightMapFalloff;
							if (density > 0.1)
							{
								_blocks[x][y][z] = (dist > 60.0 || rand() % 60 < dist) ? END_STONE : OBSIDIAN;
								placed = true;
							}
						}
					}

					// === Chaos Rings === (only if nothing placed yet)
					if (!placed)
					{
						const int outerRadius = 280;
						const int rings = 4;
						const int width = 20;
						const int spacing = 8;

						int ringDist = static_cast<int>(sqrt(worldX * worldX + worldZ * worldZ));

						for (int i = 0; i < rings && !placed; i++)
						{
							int outer = outerRadius - ((width + spacing) * i);
							int inner = outer - width;

							if (ringDist < outer && ringDist >= inner)
							{
								// Deterministic "wobble" using noise
								double wobble = sin(worldX * 0.002) + cos(worldZ * 0.002);
								int ringY = 20 + static_cast<int>(wobble * 5.0);

								// Give thickness of 3 blocks
								if (std::abs(y - ringY) <= 1)
								{
									_blocks[x][y][z] = rand() % 25 ? AIR : END_STONE;
									placed = true;
								}
							}
						}
					}

					// Default = air
					if (!placed)
						_blocks[x][y][z] = AIR;
				}
			}
		}
	}
	if (environment != THE_END)
		_generateSand(environment);
	if (environment == OVERWORLD)
		_generateStructures();
	setState(GENERATED);
}

Chunk::~Chunk()
{
	if (_vbo)
		glDeleteBuffers(1, &_vbo);
	if (_ibo)
		glDeleteBuffers(1, &_ibo);
	if (_tbo)
		glDeleteBuffers(1, &_tbo);
	if (_vao)
		glDeleteVertexArrays(1, &_vao);
}

static bool isFaceRenderable(uint8_t b1, uint8_t b2)
{
	BlockType blockToCheck   = BlockTypeRegistry::getBlockType(b1);
	BlockType blockToCompare = BlockTypeRegistry::getBlockType(b2);

	if (blockToCheck.isLiquid && blockToCompare.isLiquid)
		return false;
	if (blockToCompare.isVisible && !blockToCompare.isTransparent && !blockToCompare.isLiquid)
		return false;
	return true;
}

inline bool Chunk::_isBlockVisible(int x, int y, int z)
{
	return (x == 0 || y == 0 || z == 0
		|| x == CHUNK_WIDTH - 1 || y == CHUNK_HEIGHT - 1 || z == CHUNK_DEPTH - 1
		|| (x - 1 >= 0 && isFaceRenderable(_blocks[x][y][z], _blocks[x - 1][y][z]))
		|| (x + 1 < CHUNK_WIDTH && isFaceRenderable(_blocks[x][y][z], _blocks[x + 1][y][z]))
		|| (y - 1 >= 0 && isFaceRenderable(_blocks[x][y][z], _blocks[x][y - 1][z]))
		|| (y + 1 < CHUNK_HEIGHT && isFaceRenderable(_blocks[x][y][z], _blocks[x][y + 1][z]))
		|| (z - 1 >= 0 && isFaceRenderable(_blocks[x][y][z], _blocks[x][y][z - 1]))
		|| (z + 1 < CHUNK_DEPTH && isFaceRenderable(_blocks[x][y][z], _blocks[x][y][z + 1])));
}

bool Chunk::_isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right)
{
	BlockType blockToCompare = BlockTypeRegistry::getBlockType(AIR);
	BlockType currentBlock = BlockTypeRegistry::getBlockType(_blocks[x][y][z]);

	if (currentBlock.isLiquid && face != FACE_TOP)
		return false;
	switch (face)
	{
		case FACE_FRONT:
			if (front)
				blockToCompare = front->getBlockAtChunkLocation(Location(x, y, 0));
			if (z == CHUNK_DEPTH - 1 && front && blockToCompare.isVisible && !blockToCompare.isLiquid && !blockToCompare.isTransparent)
				return false;
			else
				return (z + 1 < CHUNK_DEPTH && isFaceRenderable(_blocks[x][y][z], _blocks[x][y][z + 1])) || z == CHUNK_DEPTH - 1;
		case FACE_BACK:
			if (back)
				blockToCompare = back->getBlockAtChunkLocation(Location(x, y, CHUNK_DEPTH - 1));
			if (z == 0 && back && blockToCompare.isVisible && !blockToCompare.isLiquid && !blockToCompare.isTransparent)
				return false;
			else
				return (z - 1 >= 0 && isFaceRenderable(_blocks[x][y][z], _blocks[x][y][z - 1])) || z == 0;
		case FACE_LEFT:
			if (left)
				blockToCompare = left->getBlockAtChunkLocation(Location(CHUNK_WIDTH - 1, y, z));
			if (x == 0 && left && blockToCompare.isVisible && !blockToCompare.isLiquid && !blockToCompare.isTransparent)
				return false;
			else
				return (x - 1 >= 0 && isFaceRenderable(_blocks[x][y][z], _blocks[x - 1][y][z])) || x == 0;
		case FACE_RIGHT:
			if (right)
				blockToCompare = right->getBlockAtChunkLocation(Location(0, y, z));
			if (x == CHUNK_WIDTH - 1 && right && blockToCompare.isVisible && !blockToCompare.isLiquid && !blockToCompare.isTransparent)
				return false;
			else
				return (x + 1 < CHUNK_WIDTH && isFaceRenderable(_blocks[x][y][z], _blocks[x + 1][y][z])) || x == CHUNK_WIDTH - 1;
		case FACE_TOP:
			return (y + 1 < CHUNK_HEIGHT && isFaceRenderable(_blocks[x][y][z], _blocks[x][y + 1][z])) || y == CHUNK_HEIGHT - 1;
		case FACE_BOTTOM:
			return (y - 1 >= 0 && isFaceRenderable(_blocks[x][y][z], _blocks[x][y - 1][z])) || y == 0;
	}
	return false;
}

void Chunk::uploadMesh()
{
	if (!_vao)
		glGenVertexArrays(1, &_vao);
	if (!_vbo)
		glGenBuffers(1, &_vbo);
	if (!_ibo)
		glGenBuffers(1, &_ibo);
	if (!_tbo)
		glGenBuffers(1, &_tbo);

	glBindVertexArray(_vao);

	// Position
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTICES_COUNT * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Texture coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTICES_COUNT * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Brightness
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTICES_COUNT * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Tints
	glBindBuffer(GL_ARRAY_BUFFER, _tbo);
	glBufferData(GL_ARRAY_BUFFER, _tints.size() * sizeof(glm::u8vec3), _tints.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(glm::u8vec3), nullptr);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint16_t), _indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_indicesSize = _indices.size();

	_tints.clear();
	_tints.shrink_to_fit();
	_vertices.clear();
	_vertices.shrink_to_fit();
	_indices.clear();
	_indices.shrink_to_fit();
	setState(UPLOADED);
}

void Chunk::unloadMesh()
{
	if (_vbo)
	{
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}
	if (_ibo)
	{
		glDeleteBuffers(1, &_ibo);
		_ibo = 0;
	}
	if (_tbo)
	{
		glDeleteBuffers(1, &_tbo);
		_tbo = 0;
	}
	if (_vao)
	{
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
	setState(GENERATED);
}

void Chunk::generateMesh()
{
	if (getState() == IDLE)
		return;
	std::vector<float> vertices;
	std::vector<uint16_t> indices;
	std::vector<glm::u8vec3> tints;
	const Object & object = ObjectRegistry::getObject(BLOCK);
	int invisibleFaces;

	if (!_world)
		handleExit(FAILURE_WORLD);
	Chunk * front = _world->getChunkAtChunkLocation(_chunkX, _chunkZ + 1);
	Chunk * back = _world->getChunkAtChunkLocation(_chunkX, _chunkZ - 1);
	Chunk * left = _world->getChunkAtChunkLocation(_chunkX - 1, _chunkZ);
	Chunk * right = _world->getChunkAtChunkLocation(_chunkX + 1, _chunkZ);

	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				invisibleFaces = 0;
				const BlockType & block = BlockTypeRegistry::getBlockType(_blocks[x][y][z]);
				if ((block.isTransparent && block.isVisible) || (block.isVisible && _isBlockVisible(x, y, z)))
				{
					std::vector<float> blockVertices = object.vertices;
					std::vector<uint16_t> blockIndices = object.indices;
					size_t vertexOffset = vertices.size() / VERTICES_COUNT; // number of vertices added so far
					// Add vertices, offsetting positions by chunk coordinates
					for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
					{
						bool faceVisible = block.isTransparent || _isFaceVisible(static_cast<BlockFace>(face), x, y, z, front, back, left, right);
						if (!faceVisible)
						{
							invisibleFaces++;
							continue;
						}
						for (int i = 0; i < 4; ++i)
						{
							uint16_t vi = (face * 4 + i) * 5;
							float vx = blockVertices[vi + 0] + x + 0.5f;
							float vy = blockVertices[vi + 1] + y;
							float vz = blockVertices[vi + 2] + z + 0.5f;

							vertices.push_back(vx);
							if ((block.type == WATER || block.type == LAVA) && _blocks[x][y + 1][z] == AIR)
								vy -= 0.125f;
							vertices.push_back(vy);
							vertices.push_back(vz);
							glm::vec2 baseUV = _world->getAtlas()->getUVForBlock(block.type, static_cast<BlockFace>(face));
							float tileSize = 1.0f / _world->getAtlas()->getTilesPerRow();
							float epsilon = 0.001f / _world->getAtlas()->getWidth();

							float localU = blockVertices[vi + 3];
							float localV = blockVertices[vi + 4];
							float finalU = baseUV.x + epsilon + localU * (tileSize - 2 * epsilon);
							float finalV = baseUV.y + epsilon + localV * (tileSize - 2 * epsilon);
							vertices.push_back(finalU);
							vertices.push_back(finalV);

							glm::vec3 normal;
							switch (face) {
								case FACE_FRONT:  normal = {0, 0, 1}; break;
								case FACE_BACK:   normal = {0, 0, -1}; break;
								case FACE_LEFT:   normal = {-1, 0, 0}; break;
								case FACE_RIGHT:  normal = {1, 0, 0}; break;
								case FACE_TOP:    normal = {0, 1, 0}; break;
								case FACE_BOTTOM: normal = {0, -1, 0}; break;
							}
							vertices.push_back(normal.x);
							vertices.push_back(normal.y);
							vertices.push_back(normal.z);
							tints.push_back(BlockTypeRegistry::getTint(block.type, static_cast<BlockFace>(face)));
						}
					}
					if (invisibleFaces == 6)
						continue;
					// Add indices with offset
					for (size_t i = 0; i < blockIndices.size(); i += 3)
					{
						indices.push_back(blockIndices[i] + vertexOffset);
						indices.push_back(blockIndices[i + 1] + vertexOffset);
						indices.push_back(blockIndices[i + 2] + vertexOffset);
					}
				}
			}
		}
	}
	_vertices = vertices;
	_indices = indices;
	_tints = tints;

	ChunkState state = getState();
	setState(state == DIRTY ? CLEANED : MESHED);
}

void Chunk::render(const Shader & shader) const
{
	if (_indicesSize == 0 || !_vao)
		return;
	glBindVertexArray(_vao);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(
		_chunkX * CHUNK_WIDTH,
		0.0f,
		_chunkZ * CHUNK_DEPTH
	));;
	shader.setMat4("model", model);
	shader.setVec3("lightDir", glm::normalize(glm::vec3(1.0f, -1.5f, 0.8f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _world->getAtlas()->getTextureID());
	shader.setInt("textureAtlas", 0);
	g_DEBUG_INFO.drawCalls++;
	if (g_DEBUG_INFO.wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, _indicesSize, GL_UNSIGNED_SHORT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
		glDrawElements(GL_TRIANGLES, _indicesSize, GL_UNSIGNED_SHORT, 0);
}

std::vector<Chunk * > Chunk::getNeighborChunks()
{
	return { _world->getChunkAtChunkLocation(_chunkX - 1, _chunkZ), _world->getChunkAtChunkLocation(_chunkX + 1, _chunkZ),
			 _world->getChunkAtChunkLocation(_chunkX, _chunkZ - 1), _world->getChunkAtChunkLocation(_chunkX, _chunkZ + 1) };
}

BlockType Chunk::getBlockAt(const Location & loc)
{
	if (getState() == IDLE)
		return BlockTypeRegistry::getBlockType(AIR);
	int localX = (static_cast<int>(std::floor(loc.getX())) % CHUNK_WIDTH + CHUNK_WIDTH) % CHUNK_WIDTH;
	int localY = static_cast<int>(std::floor(loc.getY()));
	int localZ = (static_cast<int>(std::floor(loc.getZ())) % CHUNK_DEPTH + CHUNK_DEPTH) % CHUNK_DEPTH;
	if (localX < 0 || localX >= CHUNK_WIDTH
		|| localY < 0 || localY >= CHUNK_HEIGHT
		|| localZ < 0 || localZ >= CHUNK_DEPTH)
	{
		std::cerr << "[Chunk] Warning: Requested invalid location "
			<< loc << " for Chunk(" << _chunkX * CHUNK_WIDTH << ", " << _chunkZ * CHUNK_DEPTH << "), returning air.\n";
		return BlockTypeRegistry::getBlockType(AIR);
	}
	return BlockTypeRegistry::getBlockType(
		_blocks
		[localX]
		[localY]
		[localZ]
	);
}

void Chunk::changeBlockAt(const Location & loc, Material newMaterial)
{
	int localX = (static_cast<int>(std::floor(loc.getX())) % CHUNK_WIDTH + CHUNK_WIDTH) % CHUNK_WIDTH;
	int localY = static_cast<int>(std::floor(loc.getY()));
	int localZ = (static_cast<int>(std::floor(loc.getZ())) % CHUNK_DEPTH + CHUNK_DEPTH) % CHUNK_DEPTH;
	if (localX < 0 || localX >= CHUNK_WIDTH
		|| localY < 0 || localY >= CHUNK_HEIGHT
		|| localZ < 0 || localZ >= CHUNK_DEPTH)
	{
		std::cerr << "[Chunk] Warning: Requested invalid location "
			<< loc << " for Chunk(" << _chunkX * CHUNK_WIDTH << ", " << _chunkZ * CHUNK_DEPTH << "), returning air.\n";
		return ;
	}
	_blocks[localX][localY][localZ] = newMaterial;
	setState(DIRTY);
}

BlockType Chunk::getBlockAtChunkLocation(const Location & loc)
{
	if (getState() == IDLE)
		return BlockTypeRegistry::getBlockType(AIR);
	if (loc.getX() < 0 || loc.getX() >= CHUNK_WIDTH
		|| loc.getY() < 0 || loc.getY() >= CHUNK_HEIGHT
		|| loc.getZ() < 0 || loc.getZ() >= CHUNK_DEPTH)
	{
		std::cerr << "[Chunk] Warning: Requested invalid location "
			<< loc << " for Chunk(" << _chunkX * CHUNK_WIDTH << ", " << _chunkZ * CHUNK_DEPTH << "), returning air.\n";
		return BlockTypeRegistry::getBlockType(AIR);
	}
	return BlockTypeRegistry::getBlockType(
		_blocks
		[static_cast<int>(std::floor(loc.getX()))]
		[static_cast<int>(std::floor(loc.getY()))]
		[static_cast<int>(std::floor(loc.getZ()))]
	);
}

std::ostream & operator<<(std::ostream & os, Chunk & chunk)
{
	return os << "(" << chunk.getChunkX() << ", " << chunk.getChunkZ() << ")" << " - State: " << chunk.getState();
}
