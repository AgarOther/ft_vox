/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:27:31 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/23 00:42:48 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_HPP
# define BLOCK_HPP

#include "Location.hpp"
# include "../renderer/TextureType.hpp"

class Block
{
	public:
		Block();
		Block(Material material, const Location &location, int chunkX, int chunkZ);
		Block &operator=(const Block &obj);

		void placeBlockAt(const Location &location) const;
		void place() const;
		
		Location getLocation() const;
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
};

#endif
