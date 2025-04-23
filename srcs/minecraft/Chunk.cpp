/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:20:02 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/23 17:47:26 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"
#include "World.hpp"

Chunk::Chunk(int chunkX, int chunkZ)
{
	this->_chunkX = chunkX;
	this->_chunkZ = chunkZ;
	chunkList.push_back(this);
}

Chunk::~Chunk()
{
	for (Chunk *chunk : chunkList)
		delete chunk;
}

int Chunk::getChunkX() const
{
	return (this->_chunkX);
}

int Chunk::getChunkZ() const
{
	return (this->_chunkZ);
}

// Upon program launch, generate all blocks then use placeBlockAt with its material
void Chunk::generate()
{
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++)
			{
				Material material;
				if (y == 0)
					material = BEDROCK;
				else if (y == 1)
					material = DIAMOND_ORE;
				else if (y == 2)
					material = GOLD_ORE;
				else if (y == 3)
					material = IRON_ORE;
				else if (y == 4)
					material = COAL_ORE;
				else if (y == 5)
					material = COPPER_ORE;
				else if (y == 6)
					material = EMERALD_ORE;
				else if (y == 15)
					material = GRASS_BLOCK;
				else if (y > 12)
					material = DIRT;
				else
					material = STONE;
				this->_blocks.push_back(Block(material, Location(this->_chunkX + x, y, this->_chunkZ + z), x, z));
			}
}

// Later calculations for checking for material air will include these:
// int index = x + y * chunkSize + z * chunkSize * chunkSize;
// int indexUp = x + (y + 1) * chunkSize + z * chunkSize * chunkSize;
// int indexDown = x + (y - 1) * chunkSize + z * chunkSize * chunkSize;
// int indexLeft = (x - 1) + y * chunkSize + z * chunkSize * chunkSize;
// int indexRight = (x + 1) + y * chunkSize + z * chunkSize * chunkSize;
// int indexNorth = x + y * chunkSize + (z + 1) * chunkSize * chunkSize;
// int indexSouth = x + y * chunkSize + (z - 1) * chunkSize * chunkSize;
void Chunk::draw()
{
	for (Block &block : _blocks)
		block.place();
}

void Chunk::drawAll()
{
	for (Chunk *chunk : chunkList)
		chunk->draw();
}

void Chunk::addBlock(Block &block)
{
	for (Block &chunkBlock : this->_blocks)
	{
		if (chunkBlock.getLocation() == block.getLocation())
			return (chunkBlock.setType(block.getType()));
	}
	_blocks.push_back(block);
}

std::vector<Block> &Chunk::getBlocks()
{
	return (this->_blocks);
}


Chunk *Chunk::getChunk(int chunkX, int chunkZ)
{
	for (Chunk *chunk : chunkList)
	{
		if (chunk->getChunkX() == chunkX && chunk->getChunkZ() == chunkZ)
			return (chunk);
	}
	Chunk *chunk = new Chunk(chunkX, chunkZ);
	chunkList.push_back(chunk);
	return (chunk);
}
