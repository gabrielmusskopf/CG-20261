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
    
    std::vector<Vertex> finalVertices; // Os vértices finais estruturados que irão para a Mesh

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
            // Lógica para montar os triângulos baseados nos índices do OBJ
            while (ssline >> word) {
                int vi = 0, ti = 0, ni = 0;
                std::istringstream ss(word);
                std::string index;

                if (std::getline(ss, index, '/')) vi = !index.empty() ? std::stoi(index) - 1 : 0;
                if (std::getline(ss, index, '/')) ti = !index.empty() ? std::stoi(index) - 1 : 0;
                if (std::getline(ss, index)) ni = !index.empty() ? std::stoi(index) - 1 : 0;

                Vertex v;
                v.Position = temp_vertices[vi];
                
                // Extrair texturas e normais se existirem
                if (!temp_uvs.empty() && ti >= 0) v.TexCoords = temp_uvs[ti];
                if (!temp_normals.empty() && ni >= 0) v.Normal = temp_normals[ni];

                // Cores aleatórias
                v.Color = glm::vec3(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f);

                finalVertices.push_back(v);
            }
        }
    }

    file.close();
    std::cout << "Modelo " << path << " carregado com sucesso!" << std::endl;

    // Criar o objeto Mesh e adicionar à lista de malhas do modelo
    meshes.emplace_back(finalVertices);
}
