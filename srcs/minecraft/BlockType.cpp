/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockType.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:40:25 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/23 00:07:31 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BlockType.hpp"
#include "BlockData.hpp"
#include "Block.hpp"
#include "../utils/Utils.hpp"

std::unordered_map<Material, BlockType> blockMap;

BlockType::BlockType(): _material(Material::NONE) {
}

BlockType::BlockType(const Material material)
{
	const GLuint *texIds = TextureType::getIndices(material);

	_vao = VAO();
	_vao.bind();
	_vbo = VBO(vertices, 120 * sizeof(GLfloat));
	_texVbo = VBO(texIds, 24 * sizeof(GLuint));
	_blockFacesVbo = VBO(blockFaces, 24 * sizeof(GLuint));
	_ebo = EBO(indices, 36 * sizeof(GLuint));
	_textures = TextureType::generateTextures(material);
	_material = material;

	_vao.linkAttribFloat(_vbo, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), nullptr);
	_vao.linkAttribFloat(_vbo, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
	_vao.linkAttribInt(_texVbo, 2, 1, GL_UNSIGNED_INT, sizeof(GLuint), nullptr);
	_vao.linkAttribInt(_blockFacesVbo, 3, 1, GL_UNSIGNED_INT, sizeof(GLuint), nullptr);
	_vao.unbind();
}

BlockType &BlockType::operator=(const BlockType &obj)
{
	if (&obj == this)
		return (*this);
	_vao = obj._vao;
	_vao.bind();
	_vbo = obj._vbo;
	_texVbo = obj._texVbo;
	_blockFacesVbo = obj._blockFacesVbo;
	_ebo = obj._ebo;
	_textures = obj._textures;
	_material = obj._material;
	return (*this);
}

void BlockType::draw(const Material &material, Shader &shader)
{
	if (material == Material::NONE)
		return;
	BlockType &blockType = blockMap[material];
	blockType._vao.bind();
	shader.setInt("texCount", static_cast<int>(blockType._textures.size()));
	shader.setTint(blockType._material);
	for (int i = 0; i < static_cast<int>(blockType._textures.size()); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		blockType._textures[i]->bind();
		shader.setInt(("textures[" + std::to_string(i) + "]").c_str(), i);
	}

	for (int i = 0; i <= static_cast<int>(BlockFace::DOWN); i++)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void *>(i * 6 * sizeof(GLuint)));
	Texture::resetSlots();
	blockType._vao.unbind();
}

void BlockType::free()
{
	_vbo.free();
	_texVbo.free();
	_vao.free();
	_ebo.free();
	for (Texture *_texture : _textures)
	{
		_texture->free();
		delete _texture;
	}
}

const Material &BlockType::getType() const
{
	return (_material);
}

void BlockType::init()
{
	for (int i = 0; i < static_cast<int>(UNKNOWN) + 1; i++) {
		Material m = static_cast<Material>(i);
		blockMap.insert({m, BlockType(m)});
	}
}

void BlockType::shutdown()
{
	for (int i = 0; i < static_cast<int>(UNKNOWN) + 1; i++) {
		Material m = static_cast<Material>(i);
		blockMap[m].free();
	}
	blockMap.clear();
}
