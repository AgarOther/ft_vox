/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:21:43 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/26 23:21:55 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP
#include <array>
#include "Block.hpp"

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = 32;

class Chunk {
public:
    Chunk(int chunkX, int chunkZ);
    ~Chunk();

    void render();
    void setBlock(Material material, int chunkX, int y, int chunkZ);
    static void renderAll();
private:
    std::array<Block, CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE> _blocks;
};

#endif
