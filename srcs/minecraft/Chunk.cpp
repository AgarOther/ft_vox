#include <cstdlib>
#include <mutex>
#include <vector>
#include <iostream>
#include "Chunk.hpp"
#include "ObjectRegistry.hpp"
#include "BlockTypeRegistry.hpp"
#include "StructureRegistry.hpp"
#include "Shader.hpp"
#include "errors.hpp"
#include "types.hpp"
#include "utils.hpp"

// If chunk does weird stuff, revert indices back to uint32_t

std::mutex g_debugMutex;

float frequency = 3.0f;
float amplitude = 3.f; // Max terrain height variation
int baseHeight = 64;

void Chunk::generateStructures()
{
	if (rand() % 3)
		return;
	const Structure & tree = StructureRegistry::getStructure(TREE);
	const int x = rand() % 16;
	const int z = rand() % 16;
	int y = baseHeight;
	if (x + tree.size.x >= CHUNK_WIDTH || y + tree.size.y >= CHUNK_HEIGHT || z + tree.size.z >= CHUNK_DEPTH)
		return;
	while (_blocks[x][y][z] == AIR)
		y++;

	for (std::pair<glm::vec3, Material> data : tree.data)
	{
		const int dx = x + data.first.x;
		const int dy = y + data.first.y + 1;
		const int dz = z + data.first.z;
		if (dx >= CHUNK_WIDTH || dy >= CHUNK_HEIGHT || dz >= CHUNK_DEPTH)
			continue;
		_blocks[dx][dy][dz] = data.second;
	}
}

void Chunk::generateBlocks()
{
	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				float worldX = static_cast<float>(_chunkX * CHUNK_WIDTH + x);
				float worldZ = static_cast<float>(_chunkZ * CHUNK_DEPTH + z);

				float noiseValue = _world->getNoise().GetNoise(worldX * frequency, worldZ * frequency);
				int height = static_cast<int>((noiseValue + 0.25f) * 0.5f * amplitude + baseHeight);
				int stoneOffset = static_cast<int>(floor(height / noiseValue)) % 3 + 2;

				if (y == height)
					_blocks[x][y][z] = GRASS_BLOCK;
				else if (y == 0)
					_blocks[x][y][z] = BEDROCK;
				else if (y < height && y > height - stoneOffset)
					_blocks[x][y][z] = DIRT;
				else if (y <= height - stoneOffset)
					_blocks[x][y][z] = STONE;
				else if (y > height)
					_blocks[x][y][z] = AIR;
				else
					_blocks[x][y][z] = DIRT;
			}
		}
	}
	generateStructures();
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

inline bool Chunk::isBlockVisible(int x, int y, int z)
{
	return (x == 0 || y == 0 || z == 0
		|| x == CHUNK_WIDTH - 1 || y == CHUNK_HEIGHT - 1 || z == CHUNK_DEPTH - 1
		|| (x - 1 >= 0 && BlockTypeRegistry::getBlockType(_blocks[x - 1][y][z]).isTransparent)
		|| (x + 1 < CHUNK_WIDTH && BlockTypeRegistry::getBlockType(_blocks[x + 1][y][z]).isTransparent)
		|| (y - 1 >= 0 && BlockTypeRegistry::getBlockType(_blocks[x][y - 1][z]).isTransparent)
		|| (y + 1 < CHUNK_HEIGHT && BlockTypeRegistry::getBlockType(_blocks[x][y + 1][z]).isTransparent)
		|| (z - 1 >= 0 && BlockTypeRegistry::getBlockType(_blocks[x][y][z - 1]).isTransparent)
		|| (z + 1 < CHUNK_DEPTH && BlockTypeRegistry::getBlockType(_blocks[x][y][z + 1]).isTransparent));
}

bool Chunk::isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right)
{
	switch (face)
	{
		case FACE_FRONT:
			if (z == CHUNK_DEPTH - 1 && front && front->getBlockAtChunkLocation(Location(x, y, 0)).isVisible)
				return false;
			else
				return (z + 1 < CHUNK_DEPTH && BlockTypeRegistry::getBlockType(_blocks[x][y][z + 1]).isTransparent) || z == CHUNK_DEPTH - 1;
		case FACE_BACK:
			if (z == 0 && back && back->getBlockAtChunkLocation(Location(x, y, CHUNK_DEPTH - 1)).isVisible)
				return false;
			else
				return (z - 1 >= 0 && BlockTypeRegistry::getBlockType(_blocks[x][y][z - 1]).isTransparent) || z == 0;
		case FACE_LEFT:
			if (x == 0 && left && left->getBlockAtChunkLocation(Location(CHUNK_WIDTH - 1, y, z)).isVisible)
				return false;
			else
				return (x - 1 >= 0 && BlockTypeRegistry::getBlockType(_blocks[x - 1][y][z]).isTransparent) || x == 0;
		case FACE_RIGHT:
			if (x == CHUNK_WIDTH - 1 && right && right->getBlockAtChunkLocation(Location(0, y, z)).isVisible)
				return false;
			else
				return (x + 1 < CHUNK_WIDTH && BlockTypeRegistry::getBlockType(_blocks[x + 1][y][z]).isTransparent) || x == CHUNK_WIDTH - 1;
		case FACE_TOP:
			return (y + 1 < CHUNK_HEIGHT && BlockTypeRegistry::getBlockType(_blocks[x][y + 1][z]).isTransparent) || y == CHUNK_HEIGHT - 1;
		case FACE_BOTTOM:
			return (y - 1 >= 0 && BlockTypeRegistry::getBlockType(_blocks[x][y - 1][z]).isTransparent) || y == 0;
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
				if ((block.isTransparent && block.isVisible) || (block.isVisible && isBlockVisible(x, y, z)))
				{
					std::vector<float> blockVertices = object.vertices;
					std::vector<uint16_t> blockIndices = object.indices;
					size_t vertexOffset = vertices.size() / VERTICES_COUNT; // number of vertices added so far
					// Add vertices, offsetting positions by chunk coordinates
					for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
					{
						bool faceVisible = block.isTransparent || isFaceVisible(static_cast<BlockFace>(face), x, y, z, front, back, left, right);
						if (!faceVisible)
						{
							invisibleFaces++;
							continue;
						}
						for (int i = 0; i < 4; ++i)
						{
							uint16_t vi = (face * 4 + i) * 5;
							float vx = blockVertices[vi + 0] + x;
							float vy = blockVertices[vi + 1] + y;
							float vz = blockVertices[vi + 2] + z;

							vertices.push_back(vx);
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
