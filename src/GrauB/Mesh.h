/* A Mesh representa um único objeto desenhável. 
 * A struct Vertex agrupa as propriedades matemáticas de um ponto, em vez de usar arrays matemáticos soltos.
 */
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Estrutura que define as propriedades de um único vértice
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoords; 
    glm::vec3 Normal;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    GLuint VAO;

    // Construtor gera os buffers automaticamente
    Mesh(std::vector<Vertex> vertices);
    // Destrutor limpa a VRAM
    ~Mesh();

    // Método de desenho
    void draw() const;

private:
    GLuint VBO;
    void setupMesh();
};

#endif
