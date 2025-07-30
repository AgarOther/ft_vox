#include <BlockTypeRegistry.hpp>
#include <ObjectRegistry.hpp>
#include <TextureAtlas.hpp>
#include <vector>
#include "Chunk.hpp"
#include "Shader.hpp"
#include "types.hpp"

Chunk::Chunk(int chunkX, int chunkZ): _chunkX(chunkX), _chunkZ(chunkZ), _vao(0), _vbo(0), _ibo(0)
{
	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				_blocks[x][y][z] = y <= 64 ? STONE : AIR;
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

bool Chunk::isBlockVisible(int x, int y, int z)
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
					Object object = ObjectRegistry::getObject(BLOCK);
					std::vector<float> blockVertices = object.vertices;
					std::vector<uint32_t> blockIndices = object.indices;
					size_t vertexOffset = vertices.size() / 5; // number of vertices added so far
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
						}
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr); // vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _atlas.getTextureID());
	shader.setInt("textureAtlas", 0);
	glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
}
