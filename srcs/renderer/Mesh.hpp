//
// Created by scraeyme on 4/27/25.
//

#ifndef MESH_HPP
#define MESH_HPP
#include <vector>

#include "EBO.hpp"
#include "VAO.hpp"
#include "libs/glm/vec2.hpp"
#include "libs/glm/vec3.hpp"

typedef struct s_vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    uint32_t textureId;
    uint32_t faceId;
}           Vertex;

class Mesh {
    public:
        Mesh();
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
        void clear();
        void upload();
        void draw() const;
        void addFace();
    private:
        VAO _vao;
        VBO _vbo;
        EBO _ebo;
        std::vector<Vertex> _vertices;
        std::vector<GLuint> _indices;
};

#endif
