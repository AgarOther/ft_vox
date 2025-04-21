/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 03:10:34 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/21 03:35:00 by scraeyme         ###   ########.fr       */
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
		void generate(const Block &grassBlock, const Block &dirt, const Block &bedrock, const Block &stone);
		void draw();
	private:
		std::vector<Block> _blocks;
		int _chunkX;
		int _chunkZ;
};

#endif
