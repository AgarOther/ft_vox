#include "Filter.hpp"
#include "ObjectRegistry.hpp"

Filter::Filter(): _shader(Shader("filter.vert", "filter.frag"))
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);

	Object filter = ObjectRegistry::getObject(FILTER);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, filter.vertices.size() * sizeof(float), filter.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, filter.indices.size() * sizeof(uint16_t), filter.indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_indicesSize = filter.indices.size();

	filter.vertices.clear();
	filter.vertices.shrink_to_fit();
	filter.indices.clear();
	filter.indices.shrink_to_fit();
}

Filter::~Filter()
{
	if (_vbo)
		glDeleteBuffers(1, &_vbo);
	if (_ibo)
		glDeleteBuffers(1, &_ibo);
	if (_vao)
		glDeleteVertexArrays(1, &_vao);
}

void Filter::draw(const glm::vec4 & filterColor)
{
	glBindVertexArray(_vao);
	_shader.bind();
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_shader.setVec4("filterColor", filterColor);
	glDrawElements(GL_TRIANGLES, _indicesSize, GL_UNSIGNED_SHORT, 0);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	_shader.unbind();
}
