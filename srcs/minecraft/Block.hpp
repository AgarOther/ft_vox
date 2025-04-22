/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:27:31 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 19:02:11 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_HPP
# define BLOCK_HPP

# include "BlockType.hpp"
# include "../renderer/TextureType.hpp"

class Chunk;

class Block
{
	public:
		Block();
		Block(Material material, Location location, int chunkX, int chunkZ, Chunk chunk);
		Block &operator=(const Block &obj);

		void placeBlockAt(const Location &location);
		void place();

		static Block *getBlockInChunk(Chunk &chunk, int chunkX, int y, int chunkZ);
		Block *getRelative(BlockFace face);
		
		Location getLocation() const;
		Chunk &getChunk() const;
		Location getChunkLocation() const;
		Material getType() const;
		bool isSolid() const;

		void setType(Material material);
	private:
		int _x;
		int _y;
		int _z;
		int _chunkX;
		int _chunkZ;
		Material _material;
		bool _isSolid;
		Chunk _chunk;
};

#endif
