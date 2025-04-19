/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:25 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/19 22:58:40 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"
#include "BlockData.hpp"
#include "Utils.hpp"

Block::Block(Material material)
{
	const GLuint *texIds = TextureType::getIndices(material);

	this->_vao = VAO();
	this->_vao.bind();
	this->_vbo = VBO(vertices, sizeof(vertices));
	this->_texVbo = VBO(texIds, 24 * sizeof(GLuint));
	this->_ebo = EBO(indices, sizeof(indices));
	this->_textures = TextureType::generateTextures(material);

	this->_vao.linkAttribFloat(this->_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), NULL);
	this->_vao.linkAttribFloat(this->_vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	this->_vao.linkAttribFloat(this->_vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	this->_vao.linkAttribInt(this->_texVbo, 3, 1, GL_UNSIGNED_INT, sizeof(GLuint), NULL);
	this->_vao.unbind();
}

void Block::draw(Shader &shader)
{
	this->_vao.bind();
	shader.setInt("texCount", (int)this->_textures.size());
	for (int i = 0; i < (int)this->_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		this->_textures[i]->bind();
		shader.setInt(("textures[" + std::to_string(i) + "]").c_str(), i);
	}
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
	Texture::resetSlots();
}

void Block::placeBlockAt(const Location &location)
{
	Shader &shader = Shader::getCurrentlyBoundShader();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((int)location.getX(), (int)location.getY(), (int)location.getZ()));
	shader.setMat4("model", model);
	this->draw(shader);
}

void Block::placeBlockAt(const Location &location, Shader &shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((int)location.getX(), (int)location.getY(), (int)location.getZ()));
	shader.setMat4("model", model);
	this->draw(shader);
}


void Block::free()
{
	this->_vbo.free();
	this->_texVbo.free();
	this->_vao.free();
	this->_ebo.free();
	for (int i = 0; i < (int)this->_textures.size(); i++)
	{
		this->_textures[i]->free();
		delete this->_textures[i];
	}
}
