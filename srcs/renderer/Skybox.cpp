#include "Skybox.hpp"
#include "ObjectRegistry.hpp"
#include "Shader.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "stb/stb_image.h"

Skybox::Skybox(): _vao(0), _vbo(0), _shader(Shader("skybox.vert", "skybox.frag"))
{
	std::vector<float> skyboxVertices = ObjectRegistry::getObject(SKYBOX).vertices;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
}

Skybox::~Skybox()
{
	if (_vbo)
		glDeleteBuffers(1, &_vbo);
	if (_vao)
		glDeleteVertexArrays(1, &_vao);
}

void Skybox::render(const Camera * camera, const Environment environment)
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	_shader.bind();
	_shader.setMat4("view", glm::mat4(glm::mat3(camera->getViewMatrix())));
	_shader.setMat4("projection", camera->getProjectionMatrix());
	_shader.setInt("environment", environment);

	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
