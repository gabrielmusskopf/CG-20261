/* A classe Model é a responsável por ler o arquivo do HD, extrair os dados lógicos e instanciar objetos Mesh */
#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <string>
#include <vector>

class Model {
public:
    // O construtor recebe o caminho do arquivo e já o carrega
    Model(const std::string& path);
#include "Mesh.h"
    
    // Desenha todas as malhas que compõem este modelo
    void draw() const;

private:
    std::vector<Mesh> meshes;
    void loadModel(const std::string& path);
};

#endif
