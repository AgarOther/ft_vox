#include <cstdlib>
#include <mutex>
#include <vector>
#include <iostream>
#include "Chunk.hpp"
#include "ObjectRegistry.hpp"
#include "BlockTypeRegistry.hpp"
#include "Shader.hpp"
#include "types.hpp"
#include "utils.hpp"

std::mutex g_debugMutex;
std::mutex g_printMutex;

float frequency = 0.5f;
float amplitude = 50.0f; // Max terrain height variation
int baseHeight = 64;

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

				float noiseValue = _noise.GetNoise(worldX * frequency, worldZ * frequency);
				int height = static_cast<int>((noiseValue + 0.25f) * 0.5f * amplitude + baseHeight);

				if (y < height)
					_blocks[x][y][z] = END_STONE;
				else
					_blocks[x][y][z] = AIR;
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
	return (x == 0 || y == 0 || z == 0
		|| x == CHUNK_WIDTH - 1 || y == CHUNK_HEIGHT - 1 || z == CHUNK_DEPTH - 1
		|| (x - 1 >= 0 && _blocks[x - 1][y][z] == AIR)
		|| (x + 1 < CHUNK_WIDTH && _blocks[x + 1][y][z] == AIR)
		|| (y - 1 >= 0 && _blocks[x][y - 1][z] == AIR)
		|| (y + 1 < CHUNK_HEIGHT && _blocks[x][y + 1][z] == AIR)
		|| (z - 1 >= 0 && _blocks[x][y][z - 1] == AIR)
		|| (z + 1 < CHUNK_DEPTH && _blocks[x][y][z + 1] == AIR));
}

bool Chunk::isFaceVisible(BlockFace face, int x, int y, int z, Chunk * front, Chunk * back, Chunk * left, Chunk * right)
{
	switch (face)
	{
		case FACE_FRONT:
			if (front && front->getBlockAtChunkLocation(Location(x, y, 0)).isVisible)
				return false;
			else
			{
				{
					std::lock_guard<std::mutex> lock(g_printMutex);
					std::cout << "Type: " << (z + 1 < CHUNK_DEPTH ? 0 : _blocks[x][y][z + 1]) << " (X: " << x << ", Y: " << y << ", Z: " << z + 1<< ")"  << std::endl;
				}
				return (z + 1 < CHUNK_DEPTH && _blocks[x][y][z + 1] == AIR) || z == CHUNK_DEPTH - 1;
			}
		case FACE_BACK:
			if (back && back->getBlockAtChunkLocation(Location(x, y, CHUNK_DEPTH - 1)).isVisible)
				return false;
			else
			{
				{
					std::lock_guard<std::mutex> lock(g_printMutex);
					std::cout << "Type: " << (z - 1 >= 0 ? 0 : _blocks[x][y][z - 1]) << " (X: " << x + 1 << ", Y: " << y << ", Z: " << z - 1 << ")"  << std::endl;
				}
				return (z - 1 >= 0 && _blocks[x][y][z - 1] == AIR) || z == 0;
			}
		case FACE_LEFT:
			if (left && left->getBlockAtChunkLocation(Location(CHUNK_WIDTH - 1, y, z)).isVisible)
				return false;
			else
			{
				{
					std::lock_guard<std::mutex> lock(g_printMutex);
					std::cout << "Type: " << (x - 1 >= 0 ? 0 : _blocks[x - 1][y][z]) << " (X: " << x - 1 << ", Y: " << y << ", Z: " << z << ")" << std::endl;
				}
				return (x - 1 >= 0 && _blocks[x - 1][y][z] == AIR) || x == 0;
			}
		case FACE_RIGHT:
			if (right && right->getBlockAtChunkLocation(Location(0, y, z)).isVisible)
				return false;
			else
			{
				{
					std::lock_guard<std::mutex> lock(g_printMutex);
					std::cout << "Type: " << (x + 1 < CHUNK_WIDTH ? 0 : _blocks[x + 1][y][z]) << " (X: " << x + 1 << ", Y: " << y << ", Z: " << z << ")" << std::endl;
				}
				return (x + 1 < CHUNK_WIDTH && _blocks[x + 1][y][z] == AIR) || x == CHUNK_WIDTH - 1;
			}
		case FACE_TOP:
			return (y + 1 < CHUNK_HEIGHT && _blocks[x][y + 1][z] == AIR) || y == CHUNK_HEIGHT - 1;
		case FACE_BOTTOM:
			return (y - 1 >= 0 && _blocks[x][y - 1][z] == AIR) || y == 0;
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

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), _indices.data(), GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTICES_COUNT * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Texture coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTICES_COUNT * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Brightness
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTICES_COUNT * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static const std::string getFaceAsString(int face)
{
	switch (face)
	{
		case FACE_BACK: return "BACK";
		case FACE_FRONT: return "FRONT";
		case FACE_LEFT: return "LEFT";
		case FACE_RIGHT: return "RIGHT";
		case FACE_TOP: return "TOP";
		case FACE_BOTTOM: return "BOTTOM";
		default:
			return "UNKNOWN";
	}
}

void Chunk::generateMesh(const TextureAtlas & atlas, World * world)
{
	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	const Object & object = ObjectRegistry::getObject(BLOCK);
	int invisibleFaces;

	if (!world)
		handleExit(7, "MDRRRRRRR");
	Chunk * front = world->getChunkAtChunkLocation(_chunkX, _chunkZ + 1);
	Chunk * back = world->getChunkAtChunkLocation(_chunkX, _chunkZ - 1);
	Chunk * left = world->getChunkAtChunkLocation(_chunkX - 1, _chunkZ);
	Chunk * right = world->getChunkAtChunkLocation(_chunkX + 1, _chunkZ);

	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				invisibleFaces = 0;
				const BlockType & block = BlockTypeRegistry::getBlockType(_blocks[x][y][z]);
				if (block.isVisible && isBlockVisible(x, y, z))
				{
					{
						std::lock_guard<std::mutex> lock(g_debugMutex);
						g_DEBUG_INFO.blocks++;
					}
					std::vector<float> blockVertices = object.vertices;
					std::vector<uint32_t> blockIndices = object.indices;
					size_t vertexOffset = vertices.size() / VERTICES_COUNT; // number of vertices added so far
					// Add vertices, offsetting positions by chunk coordinates
					for (int face = FACE_FRONT; face <= FACE_BOTTOM; ++face)
					{
						bool faceVisible = isFaceVisible(static_cast<BlockFace>(face), x, y, z, front, back, left, right);
						{
							std::lock_guard<std::mutex> lock(g_printMutex);
							std::cout << "Face " << (faceVisible ? "VISIBLE: " : "NOTSHOWN: ") << " " << getFaceAsString(face) << " (X: " << x << ",Y: " << y << ", Z: " << z << ")" << std::endl << std::endl;
						}
						if (!faceVisible)
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
							float epsilon = 0.2f / atlas.getWidth();

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
						{
							std::lock_guard<std::mutex> lock(g_debugMutex);
							g_DEBUG_INFO.triangles += 2;
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
	_indicesCount = indices.size();
	_atlas = atlas;
	_vertices = vertices;
	_indices = indices;
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
	if (g_DEBUG_INFO.wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
		glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
}

BlockType Chunk::getBlockAt(const Location & loc)
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
		return BlockTypeRegistry::getBlockType(AIR);
	}
	return BlockTypeRegistry::getBlockType(
		_blocks
		[localX]
		[localY]
		[localZ]
	);
}

BlockType Chunk::getBlockAtChunkLocation(const Location & loc)
{
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
