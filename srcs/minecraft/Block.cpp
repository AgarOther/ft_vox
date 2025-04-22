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

#include "Block.hpp"
#include "Chunk.hpp"
#include "colors.hpp"
#include <iostream>

Block::Block(Material material, Location location, int chunkX, int chunkZ)
{
	this->_x = location.getX();
	this->_y = location.getY();
	this->_z = location.getZ();
	this->_isSolid = true;
	this->_material = material;
	this->_chunkX = chunkX;
	this->_chunkZ = chunkZ;
}

Block &Block::operator=(const Block &obj)
{
	if (&obj == this)
		return (*this);
	this->_x = obj._x;
	this->_y = obj._y;
	this->_z = obj._z;
	this->_isSolid = obj._isSolid;
	this->_material = obj._material;
	this->_chunkX = obj._chunkX;
	this->_chunkZ = obj._chunkZ;
	return (*this);
}

Location Block::getLocation() const
{
	return (Location(this->_x, this->_y, this->_z));
}

Material Block::getType() const
{
	return (this->_material);
}

bool Block::isSolid() const
{
	return (this->_isSolid);
}

void Block::setType(Material material)
{
	this->_material = material;
}

Location Block::getChunkLocation() const
{
	return (Location(this->_chunkX, this->_y, this->_chunkZ));
}

void Block::placeBlockAt(const Location &location)
{
	Shader &shader = Shader::getCurrentlyBoundShader();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((int)location.getX(), (int)location.getY(), (int)location.getZ()));
	shader.setMat4("model", model);
	BlockType::draw(this->_material, shader);
}

void Block::place()
{
	Location location(this->_x, this->_y, this->_z);
	Shader &shader = Shader::getCurrentlyBoundShader();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((int)location.getX(), (int)location.getY(), (int)location.getZ()));
	shader.setMat4("model", model);
	BlockType::draw(this->_material, shader);
}
