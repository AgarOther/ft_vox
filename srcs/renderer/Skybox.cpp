#include "Skybox.hpp"
#include "ObjectRegistry.hpp"
#include "Shader.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "stb/stb_image.h"

Skybox::Skybox(): _vao(0), _vbo(0), _textureID(0), _shader(Shader("skybox.vert", "skybox.frag"))
{
	std::vector<float> skyboxVertices = ObjectRegistry::getObject(SKYBOX).vertices;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);

	std::vector<std::string> faces = {
		"assets/skybox/right.png",   // +X
		"assets/skybox/left.png",    // -X
		"assets/skybox/top.png",     // +Y
		"assets/skybox/bottom.png",  // -Y
		"assets/skybox/front.png",   // +Z
		"assets/skybox/back.png"     // -Z
	};

	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		int width, height, channels;
		stbi_uc * data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
			handleExit(FAILURE_SKYBOX);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Skybox::~Skybox()
{
	if (_vao)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo)
		glDeleteBuffers(1, &_vbo);
	if (_textureID)
		glDeleteTextures(1, &_textureID);
}

void Skybox::render(const Camera * camera)
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	_shader.bind();
	_shader.setMat4("view", glm::mat4(glm::mat3(camera->getViewMatrix())));
	_shader.setMat4("projection", camera->getProjectionMatrix());

	glBindVertexArray(_vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
