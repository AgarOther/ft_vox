/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:25 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 03:28:29 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"
#include "BlockData.hpp"
#include "Utils.hpp"

Block::Block()
{
	
}

Block::Block(Material material)
{
	const GLuint *texIds = TextureType::getIndices(material);

	this->_vao = VAO();
	this->_vao.bind();
	this->_vbo = VBO(vertices, 120 * sizeof(GLfloat));
	this->_texVbo = VBO(texIds, 24 * sizeof(GLuint));
	this->_blockFacesVbo = VBO(blockFaces, 24 * sizeof(GLuint));
	this->_ebo = EBO(indices, 36 * sizeof(GLuint));
	this->_textures = TextureType::generateTextures(material);
	this->_material = material;

	this->_vao.linkAttribFloat(this->_vbo, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), NULL);
	this->_vao.linkAttribFloat(this->_vbo, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	this->_vao.linkAttribInt(this->_texVbo, 2, 1, GL_UNSIGNED_INT, sizeof(GLuint), NULL);
	this->_vao.linkAttribInt(this->_blockFacesVbo, 3, 1, GL_UNSIGNED_INT, sizeof(GLuint), NULL);
	this->_vao.unbind();
}

Block &Block::operator=(const Block &obj)
{
	if (&obj == this)
		return (*this);
	this->_vao = obj._vao;
	this->_vao.bind();
	this->_vbo = obj._vbo;
	this->_texVbo = obj._texVbo;
	this->_blockFacesVbo = obj._blockFacesVbo;
	this->_ebo = obj._ebo;
	this->_textures = obj._textures;
	this->_material = obj._material;
	return (*this);
}

void Block::draw(Shader &shader)
{
	this->_vao.bind();
	shader.setInt("texCount", (int)this->_textures.size());
	shader.setTint(this->_material);
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

const Material &Block::getType() const
{
	return (this->_material);
}

void Block::setType(Material &material)
{
	this->_material = material;
}
