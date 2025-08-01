#include <cstdlib>
#include <vector>
#include <iostream>
#include "Chunk.hpp"
#include "ObjectRegistry.hpp"
#include "BlockTypeRegistry.hpp"
#include "Shader.hpp"
#include "types.hpp"
#include "utils.hpp"

float frequency = 0.5f;
float amplitude = 50.0f; // Max terrain height variation
int baseHeight = 64;

Chunk::Chunk(int chunkX, int chunkZ, const FastNoiseLite & noise): _chunkX(chunkX), _chunkZ(chunkZ), _vao(0), _vbo(0), _ibo(0)
{
	
	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				float worldX = static_cast<float>(_chunkX * CHUNK_WIDTH + x);
				float worldZ = static_cast<float>(_chunkZ * CHUNK_DEPTH + z);

				float noiseValue = noise.GetNoise(worldX * frequency, worldZ * frequency);
				int height = static_cast<int>((noiseValue + 1.0f) * 0.5f * amplitude + baseHeight);

				if (y < height)
					_blocks[x][y][z] = END_STONE;
				else
					_blocks[x][y][z] = AIR;
				// if (y == 0)
                // 	_blocks[x][y][z] = BEDROCK;
				// else if (y < height - 5)
				// 	_blocks[x][y][z] = STONE;
				// else if (y < height)
				// 	_blocks[x][y][z] = SAND;
				// else
				// 	_blocks[x][y][z] = AIR;
				
			}
		}
	}
}

Chunk::~Chunk()
{
	if (_vbo)
		glDeleteBuffers(1, &_vbo);
	if (_vao)
		glDeleteVertexArrays(1, &_vao);
	if (_ibo)
		glDeleteBuffers(1, &_ibo);
}

inline bool Chunk::isBlockVisible(int x, int y, int z)
{
	return x == 0 || y == 0 || z == 0
		|| x == CHUNK_WIDTH - 1 || y == CHUNK_HEIGHT - 1 || z == CHUNK_DEPTH - 1
		|| (x - 1 >= 0 && _blocks[x - 1][y][z] == AIR)
		|| (x + 1 < CHUNK_WIDTH && _blocks[x + 1][y][z] == AIR)
		|| (y - 1 >= 0 && _blocks[x][y - 1][z] == AIR)
		|| (y + 1 < CHUNK_HEIGHT && _blocks[x][y + 1][z] == AIR)
		|| (z - 1 >= 0 && _blocks[x][y][z - 1] == AIR)
		|| (z + 1 < CHUNK_DEPTH && _blocks[x][y][z + 1] == AIR);
}

bool Chunk::isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right)
{
	switch (face)
	{
		case FACE_FRONT:
			if (z + 1 < CHUNK_DEPTH)
				return _blocks[x][y][z + 1] == AIR;
			else if (front)
				return front->getBlockAt(Location(x, y, 0)).type == AIR;
			else
				return true;

		case FACE_BACK:
			if (z - 1 >= 0)
				return _blocks[x][y][z - 1] == AIR;
			else if (back)
				return back->getBlockAt(Location(x, y, CHUNK_DEPTH - 1)).type == AIR;
			else
				return true;

		case FACE_LEFT:
			if (x - 1 >= 0)
				return _blocks[x - 1][y][z] == AIR;
			else if (left)
				return left->getBlockAt(Location(CHUNK_WIDTH - 1, y, z)).type == AIR;
			else
				return true;

		case FACE_RIGHT:
			if (x + 1 < CHUNK_WIDTH)
				return _blocks[x + 1][y][z] == AIR;
			else if (right)
				return right->getBlockAt(Location(0, y, z)).type == AIR;
			else
				return true;

		case FACE_TOP:
			return (y + 1 >= CHUNK_HEIGHT) || (_blocks[x][y + 1][z] == AIR);

		case FACE_BOTTOM:
			return (y - 1 < 0) || (_blocks[x][y - 1][z] == AIR);
	}
	return false;
}

void Chunk::generateMesh(const TextureAtlas & atlas, World * world)
{
	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	const uint8_t verticesCount = 8;
	Object object = ObjectRegistry::getObject(BLOCK);
	int invisibleFaces;

	if (!world)
		handleExit(7, "MDRRRRRRR");
	Chunk * front = world->getChunkAt(_chunkX, _chunkZ + 1);
	Chunk * back = world->getChunkAt(_chunkX, _chunkZ - 1);
	Chunk * left = world->getChunkAt(_chunkX - 1, _chunkZ);
	Chunk * right = world->getChunkAt(_chunkX + 1, _chunkZ);

	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				invisibleFaces = 0;
				uint8_t blockID = _blocks[x][y][z];
				const BlockType & block = BlockTypeRegistry::getBlockType(blockID);
				if (block.isVisible && isBlockVisible(x, y, z))
				{
					g_DEBUG_INFO.blocks++;
					std::vector<float> blockVertices = object.vertices;
					std::vector<uint32_t> blockIndices = object.indices;
					size_t vertexOffset = vertices.size() / verticesCount; // number of vertices added so far
					// Add vertices, offsetting positions by chunk coordinates
					for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
					{
						if (!isFaceVisible(static_cast<BlockFace>(face), x, y, z, front, back, left, right))
						{
							invisibleFaces++;
							continue;
						}
						for (int i = 0; i < 4; ++i)
						{
							size_t vi = (face * 4 + i) * 5;
							float vx = blockVertices[vi + 0] + x;
							float vy = blockVertices[vi + 1] + y;
							float vz = blockVertices[vi + 2] + z;

							vertices.push_back(vx);
							vertices.push_back(vy);
							vertices.push_back(vz);
							glm::vec2 baseUV = atlas.getUVForBlock(block.type);
							float tileSize = 1.0f / atlas.getTilesPerRow();
							float epsilon = 0.15f / atlas.getWidth();

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
						}
						g_DEBUG_INFO.triangles += 2;
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
	_indicesCount = indices.size();
	if (!_vao)
		glGenVertexArrays(1, &_vao);
	if (!_vbo)
		glGenBuffers(1, &_vbo);
	if (!_ibo)
		glGenBuffers(1, &_ibo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, verticesCount * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Texture coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, verticesCount * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Brightness
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, verticesCount * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	_atlas = atlas;
}

void Chunk::render(const Shader & shader) const
{
	if (_indicesCount == 0)
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
	glBindTexture(GL_TEXTURE_2D, _atlas.getTextureID());
	shader.setInt("textureAtlas", 0);
	g_DEBUG_INFO.drawCalls++;
	glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
}

BlockType Chunk::getBlockAt(const Location & loc)
{
	int localX = ((static_cast<int>(loc.getX()) % CHUNK_WIDTH) + CHUNK_WIDTH) % CHUNK_WIDTH;
    int localY = static_cast<int>(loc.getY());
    int localZ = ((static_cast<int>(loc.getZ()) % CHUNK_DEPTH) + CHUNK_DEPTH) % CHUNK_DEPTH;
	if (localX < 0 || localX >= CHUNK_WIDTH
		|| localY < 0 || localY >= CHUNK_HEIGHT
		|| localZ < 0 || localZ >= CHUNK_DEPTH)
	{
		std::cerr << "[Chunk] Warning: Requested invalid location " << loc << " for Chunk(" << _chunkX * CHUNK_WIDTH << ", " << _chunkZ * CHUNK_DEPTH << "), returning air.\n";
		return BlockTypeRegistry::getBlockType(AIR);
	}
	return BlockTypeRegistry::getBlockType(
		_blocks
		[localX]
		[localY]
		[localZ]
	);
}
