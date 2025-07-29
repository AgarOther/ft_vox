#include <BlockTypeRegistry.hpp>
#include <ObjectRegistry.hpp>
#include <vector>
#include "Chunk.hpp"

Chunk::Chunk(int chunkX, int chunkZ): _chunkX(chunkX), _chunkZ(chunkZ), _vao(0), _vbo(0), _ibo(0)
{
	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int y = 0; y < CHUNK_HEIGHT; ++y)
		{
			for (int z = 0; z < CHUNK_DEPTH; ++z)
			{
				_blocks[x][y][z] = y <= 64 ? 1 : 0;
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
}

void Chunk::generateMesh()
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
				if (block.isVisible)
				{
					// culling to be done
					Object object = ObjectRegistry::getObject(blockID);
					std::vector<float> blockVertices = object.vertices;
					std::vector<uint32_t> blockIndices = object.indices;
					size_t vertexOffset = vertices.size() / 5; // number of vertices added so far

					// Add vertices, offsetting positions by chunk coordinates
					for (size_t i = 0; i < blockVertices.size(); i += 5)
					{
						float vx = blockVertices[i + 0] + x;
						float vy = blockVertices[i + 1] + y;
						float vz = blockVertices[i + 2] + z;

						vertices.push_back(vx);
						vertices.push_back(vy);
						vertices.push_back(vz);
						vertices.push_back(blockVertices[i + 3]); // u
						vertices.push_back(blockVertices[i + 4]); // v
					}

					// Add indices with offset
					for (size_t i = 0; i < blockIndices.size(); i += 3)
					{
						indices.push_back(blockIndices[i + 0] + vertexOffset);
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr); // once textures are done
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
}
