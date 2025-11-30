#include "Crosshair.hpp"
#include "ObjectRegistry.hpp"

Crosshair::Crosshair(): _shader("crosshair.vert", "crosshair.frag")
{
	Object crosshair = ObjectRegistry::getObject(CROSSHAIR);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, crosshair.vertices.size() * sizeof(float), crosshair.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

Crosshair::~Crosshair()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_id);
}

void Crosshair::draw(float aspectRatio) const
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_shader.bind();

	_shader.setFloat("aspectRatio", aspectRatio);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, 4);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
