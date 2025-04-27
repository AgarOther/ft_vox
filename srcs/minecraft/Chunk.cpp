/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scraeyme <scraeyme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:21:36 by scraeyme          #+#    #+#             */
/*   Updated: 2025/04/26 23:21:40 by scraeyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include "srcs/utils/Utils.hpp"
#include "World.hpp"

Chunk::Chunk(const int chunkX, const int chunkZ) {
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_HEIGHT; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                const Block block(Material::BEDROCK, Location(x + chunkX, y, z + chunkZ), chunkX, chunkZ);
                const int index = Utils::getBlockIndex(x, y, z);
                _blocks[index] = block;
                _faceMasks[index] = 0;
            }
        }
    }
    update();
    WORLD_CHUNKS.push_back(this);
}

uint8_t Chunk::getFaceMask(const Block &block) const {
    (void) block;
    uint8_t mask = 63;
    return (mask);
}

void Chunk::update() {
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_HEIGHT; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                const int index = Utils::getBlockIndex(x, y, z);
                _faceMasks[index] = getFaceMask(_blocks[index]);
            }
        }
    }
}

Chunk::~Chunk() {
    for (Block &block : _blocks)
        block.~Block();
}

void Chunk::deleteAll() {
    for (const Chunk *chunk : WORLD_CHUNKS)
        delete chunk;
}

void Chunk::setBlock(const Material material, const int chunkX, const int y, const int chunkZ) {
    const int index = Utils::getBlockIndex(chunkX, y, chunkZ);
    if (index >= CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE || index < 0) return;
    Block &block = _blocks[index];
    block.setType(material);
}

void Chunk::render() {
    _mesh.draw();
}

void Chunk::renderAll() {
    for (Chunk *chunk : WORLD_CHUNKS)
        chunk->render();
}
