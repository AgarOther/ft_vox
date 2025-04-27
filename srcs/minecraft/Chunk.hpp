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
#include "renderer/Mesh.hpp"

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = 16;
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE;

class Chunk {
public:
    Chunk(int chunkX, int chunkZ);
    ~Chunk();

    static void deleteAll();
    void render();
    void update();
    void setBlock(Material material, int chunkX, int y, int chunkZ);
    uint8_t getFaceMask(const Block &block) const;
    static void renderAll();
private:
    std::array<Block, CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE> _blocks;
    std::array<uint8_t, CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE> _faceMasks;
    Mesh _mesh;
};

#endif
