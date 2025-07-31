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

				int xd = rand() % 5;
				if (y < height)
				{
					switch (xd)
					{
						case 0:
							_blocks[x][y][z] = BEDROCK;
							break;
						case 1:
							_blocks[x][y][z] = STONE;
							break;
						case 2:
							_blocks[x][y][z] = END_STONE;
							break;
						case 3:
							_blocks[x][y][z] = STONE;
							break;
						case 4:
							_blocks[x][y][z] = DIRT;
							break;
						default:
							_blocks[x][y][z] = SAND;
					}
				}
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

bool Chunk::isFaceVisible(BlockFace face, int x, int y, int z)
{
	switch (face)
	{
		case FACE_FRONT:
			return (z + 1 < CHUNK_DEPTH && _blocks[x][y][z + 1] == AIR) || z == CHUNK_DEPTH - 1;
		case FACE_BACK:
			return (z - 1 >= 0 && _blocks[x][y][z - 1] == AIR) || z == 0;
		case FACE_LEFT:
			return (x - 1 >= 0 && _blocks[x - 1][y][z] == AIR) || x == 0;
		case FACE_RIGHT:
			return (x + 1 < CHUNK_WIDTH && _blocks[x + 1][y][z] == AIR) || x == CHUNK_WIDTH - 1;
		case FACE_TOP:
			return (y + 1 < CHUNK_HEIGHT && _blocks[x][y + 1][z] == AIR) || y == CHUNK_HEIGHT - 1;
		case FACE_BOTTOM:
			return (y - 1 >= 0 && _blocks[x][y - 1][z] == AIR) || y == 0;
	}
	return false;
}

void Chunk::generateMesh(const TextureAtlas & atlas)
{
	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	const uint8_t verticesCount = 8;

	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				uint8_t blockID = _blocks[x][y][z];
				const BlockType & block = BlockTypeRegistry::getBlockType(blockID);
				if (block.isVisible && isBlockVisible(x, y, z))
				{
					g_DEBUG_INFO.blocks++;
					Object object = ObjectRegistry::getObject(BLOCK);
					std::vector<float> blockVertices = object.vertices;
					std::vector<uint32_t> blockIndices = object.indices;
					size_t vertexOffset = vertices.size() / verticesCount; // number of vertices added so far
					// Add vertices, offsetting positions by chunk coordinates
					for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
					{
						if (!isFaceVisible(static_cast<BlockFace>(face), x, y, z))
							continue;
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
							float epsilon = 0.01f / atlas.getWidth();

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
	if (loc.getX() < 0 || loc.getX() >= CHUNK_WIDTH
		|| loc.getY() < 0 || loc.getY() >= CHUNK_HEIGHT
		|| loc.getZ() < 0 || loc.getZ() >= CHUNK_DEPTH)
	{
		std::cerr << "[Chunk] Warning: Requested invalid location " << loc << " for Chunk(" << _chunkX << ", " << _chunkZ << "), returning default (0, 0, 0).\n";
		return BlockTypeRegistry::getBlockType(_blocks[0][0][0]);
	}
	return BlockTypeRegistry::getBlockType(
		_blocks
		[static_cast<int>(loc.getX()) % CHUNK_WIDTH]
		[static_cast<int>(loc.getY()) % CHUNK_HEIGHT]
		[static_cast<int>(loc.getZ()) % CHUNK_DEPTH]
	);
}
