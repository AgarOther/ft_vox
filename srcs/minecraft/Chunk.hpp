/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:10:34 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/22 03:37:27 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "Block.hpp"
# include <vector>

class Chunk
{
	public:
		Chunk(int chunkX, int chunkZ);
		
		int getChunkX() const;
		int getChunkZ() const;
		static Chunk *getChunk(int chunkX, int chunkZ);
		static void drawAll();
		
		void addBlock(Block &block);
		void generate();
		void draw();
		
		std::vector<Block> getBlocks() const;
	private:
		std::vector<Block> _blocks;
		int _chunkX;
		int _chunkZ;
};

#endif
