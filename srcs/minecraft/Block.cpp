/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:27:24 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/23 00:42:43 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/transform.hpp>
#include "Block.hpp"
#include "BlockType.hpp"
#include "srcs/renderer/Shader.hpp"

Block::Block() {
	_x = 0;
	_y = 0;
	_z = 0;
	_isSolid = false;
	_material = AIR;
	_chunkX = 0;
	_chunkZ = 0;
}

Block::Block(const Material material, const Location &location, const int chunkX, const int chunkZ)
{
	_x = static_cast<int>(location.getX());
	_y = static_cast<int>(location.getY());
	_z = static_cast<int>(location.getZ());
	_isSolid = material != Material::AIR;
	_material = material;
	_chunkX = chunkX;
	_chunkZ = chunkZ;
}

Block &Block::operator=(const Block &obj)
{
	if (&obj == this)
		return (*this);
	_x = obj._x;
	_y = obj._y;
	_z = obj._z;
	_isSolid = obj._isSolid;
	_material = obj._material;
	_chunkX = obj._chunkX;
	_chunkZ = obj._chunkZ;
	return (*this);
}

Location Block::getLocation() const
{
	return (Location(_x, _y, _z));
}

Material Block::getType() const
{
	return (_material);
}

bool Block::isSolid() const
{
	return (_isSolid);
}

void Block::setType(Material material)
{
	_material = material;
}

Location Block::getChunkLocation() const
{
	return (Location(_chunkX, _y, _chunkZ));
}

void Block::placeBlockAt(const Location &location, const uint8_t faceMask) const
{
	if (_material == AIR)
		return;
	Shader &shader = Shader::getCurrentlyBoundShader();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(static_cast<int>(location.getX()), static_cast<int>(location.getY()), static_cast<int>(location.getZ())));
	shader.setMat4("model", model);
	BlockType::draw(_material, shader, faceMask);
}

void Block::place(const uint8_t faceMask) const
{
	if (_material == AIR)
		return;
	const Location location(_x, _y, _z);
	Shader &shader = Shader::getCurrentlyBoundShader();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(static_cast<int>(location.getX()), static_cast<int>(location.getY()), static_cast<int>(location.getZ())));
	shader.setMat4("model", model);
	BlockType::draw(_material, shader, faceMask);
}
