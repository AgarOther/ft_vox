/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockType.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:25 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 01:31:24 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BlockType.hpp"
#include "BlockData.hpp"
#include "Utils.hpp"

std::unordered_map<Material, BlockType> blockMap;

BlockType::BlockType()
{
	
}

BlockType::BlockType(Material material)
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

BlockType &BlockType::operator=(const BlockType &obj)
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

void BlockType::draw(Material &material, Shader &shader)
{
	BlockType &block = blockMap[material];
	block._vao.bind();
	shader.setInt("texCount", (int)block._textures.size());
	shader.setTint(block._material);
	for (int i = 0; i < (int)block._textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		block._textures[i]->bind();
		shader.setInt(("textures[" + std::to_string(i) + "]").c_str(), i);
	}
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
	Texture::resetSlots();
	block._vao.unbind();
}

void BlockType::free()
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

const Material &BlockType::getType() const
{
	return (this->_material);
}

void BlockType::init()
{
	for (int i = 0; i < static_cast<int>(Material::UNKNOWN) + 1; ++i) {
		Material m = static_cast<Material>(i);
		blockMap.insert({m, BlockType(m)});
	}
}

void BlockType::shutdown()
{
	for (int i = 0; i < static_cast<int>(Material::UNKNOWN) + 1; ++i) {
		Material m = static_cast<Material>(i);
		blockMap[m].free();
	}
	blockMap.clear();
}
