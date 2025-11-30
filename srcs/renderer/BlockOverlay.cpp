#include "BlockOverlay.hpp"
#include "ObjectRegistry.hpp"

BlockOverlay::BlockOverlay(Camera * camera): _camera(camera), _shader(Shader("blockoverlay.vert", "blockoverlay.frag"))
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);

	Object blockOverlay = ObjectRegistry::getObject(BLOCK_OVERLAY);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, blockOverlay.vertices.size() * sizeof(float), blockOverlay.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, blockOverlay.indices.size() * sizeof(uint16_t), blockOverlay.indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_indicesSize = blockOverlay.indices.size();

	blockOverlay.vertices.clear();
	blockOverlay.vertices.shrink_to_fit();
	blockOverlay.indices.clear();
	blockOverlay.indices.shrink_to_fit();
}

BlockOverlay::~BlockOverlay()
{
	if (_vbo)
		glDeleteBuffers(1, &_vbo);
	if (_ibo)
		glDeleteBuffers(1, &_ibo);
	if (_vao)
		glDeleteVertexArrays(1, &_vao);
}

void BlockOverlay::draw(const Block & targetedBlock, float deltaTime)
{
	const float minOpacity = 0.05f;
	const float maxOpacity = 0.2f;
	const float speed = 0.4f;
	static float direction = 1.0f;
	static float opacity = 0.2f;

	if (!targetedBlock.blockType.isSolid)
		return;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), targetedBlock.position.clone().add(0.5, -0.002, 0.5).getVec3());
	model = glm::scale(model, glm::vec3(1.005f));
	_camera->setupMatrix(_shader);
	_shader.setMat4("model", model);
	_shader.setFloat("opacity", opacity);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indicesSize, GL_UNSIGNED_SHORT, 0);
	glDisable(GL_BLEND);
	_shader.unbind();

	opacity += direction * speed * deltaTime;

	if (opacity >= maxOpacity)
	{
		opacity = maxOpacity;
		direction = -direction;
	}
	else if (opacity <= minOpacity)
	{
		opacity = minOpacity;
		direction = -direction;
	}
}
