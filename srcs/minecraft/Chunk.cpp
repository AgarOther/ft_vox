/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:20:02 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 04:13:43 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include "BlockData.hpp"

Chunk::Chunk(int chunkX, int chunkZ)
{
	this->_chunkX = chunkX;
	this->_chunkZ = chunkZ;
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
void Chunk::generate(const Block &grassBlock, const Block &dirt, const Block &bedrock, const Block &stone)
{
	for (float x = 0; x < 16; x++)
		for (float y = 0; y < 16; y++)
			for (float z = 0; z < 16; z++)
			{
				Block block;
				if (y == 15)
					block = grassBlock;
				else if (y > 12)
					block = dirt;
				else if (y == 0)
					block = bedrock;
				else
					block = stone;
				_blocks.push_back(block);
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
	for (float x = 0; x < 16; x++)
		for (float y = 0; y < 16; y++)
			for (float z = 0; z < 16; z++)
			{
				Location loc(x + this->_chunkX, y, z + this->_chunkZ);
				int index = x + y * chunkSize + z * chunkSize * chunkSize;
				Block &block = _blocks[index];
				// Lol strat to optimize before culling optimization
				if ((x == 0 || x == 15) || (z == 0 || z == 15) || y == 0 || y == 15)
					block.placeBlockAt(loc);
			}
}
