#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>

Model::Model(const std::string& path) {
    loadModel(path);
}

void Model::draw() const {
    for (const Mesh& mesh : meshes) {
        mesh.draw();
    }
}

void Model::loadModel(const std::string& path) {
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    
    std::vector<Vertex> finalVertices; 

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erro ao tentar ler o modelo: " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ssline(line);
        std::string word;
        ssline >> word;

        if (word == "v") {
            glm::vec3 vertex;
            ssline >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } 
        else if (word == "vt") {
            glm::vec2 uv;
            ssline >> uv.s >> uv.t;
            temp_uvs.push_back(uv);
        } 
        else if (word == "vn") {
            glm::vec3 normal;
            ssline >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } 
        else if (word == "f") {
            // Lógica para ler faces no formato v/vt/vn ou v//vn
            while (ssline >> word) {
                int vi = -1, ti = -1, ni = -1;
                std::istringstream ss(word);
                std::string index;

                // Índice do Vértice (v)
                if (std::getline(ss, index, '/')) vi = !index.empty() ? std::stoi(index) - 1 : -1;
                
                // Índice da Textura (vt)
                if (std::getline(ss, index, '/')) ti = !index.empty() ? std::stoi(index) - 1 : -1;
                
                // Índice da Normal (vn)
                if (std::getline(ss, index)) ni = !index.empty() ? std::stoi(index) - 1 : -1;

                // Montagem do Vértice Estruturado
                Vertex v;
                
                // Garante que o vértice existe
                if (vi >= 0 && vi < temp_vertices.size()) {
                    v.Position = temp_vertices[vi];
                }

                // Garante que a textura existe, senão zera
                if (ti >= 0 && ti < temp_uvs.size()) {
                    v.TexCoords = temp_uvs[ti];
                } else {
                    v.TexCoords = glm::vec2(0.0f, 0.0f);
                }

                // Garante que a normal existe
                if (ni >= 0 && ni < temp_normals.size()) {
                    v.Normal = temp_normals[ni];
                } else {
                    v.Normal = glm::vec3(0.0f, 1.0f, 0.0f); // Normal padrão apontando para cima
                }

                // Cor base sólida
                v.Color = glm::vec3(0.8f, 0.8f, 0.8f);

                finalVertices.push_back(v);
            }
        }
    }

    file.close();
    std::cout << "Modelo '" << path << "' carregado! Vértices extraídos: " << finalVertices.size() << std::endl;

    meshes.emplace_back(finalVertices);
}
