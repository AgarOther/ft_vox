//
// Created by scraeyme on 4/27/25.
//

#include "Mesh.hpp"

#include "srcs/minecraft/BlockData.hpp"

Mesh::Mesh() {

}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
    _indices = indices;
    _vertices = vertices;
}

void Mesh::clear() {
    _indices.clear();
    _vertices.clear();
}

void Mesh::upload() {
    _vao = VAO();
    _vao.bind();
    _vbo = VBO(_vertices.data(), _vertices.size() * sizeof(Vertex));
    _ebo = EBO(_indices.data(), _indices.size() * sizeof(GLuint));
    _vao.linkAttribFloat(_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
    _vao.linkAttribFloat(_vbo, 1, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoords)));
    _vao.linkAttribInt(_vbo, 2, 1, GL_UNSIGNED_INT, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, textureId)));
    _vao.linkAttribInt(_vbo, 3, 1, GL_UNSIGNED_INT, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, faceId)));
    _vao.unbind();
}

void Mesh::draw() const {
    _vao.bind();
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
    _vao.unbind();
}
