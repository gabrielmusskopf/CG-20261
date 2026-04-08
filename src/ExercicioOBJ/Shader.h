#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
    GLuint ID; // ID do programa de shader no OpenGL

    // Construtor lê os arquivos e constrói o shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // Destrutor limpa a memória da GPU
    ~Shader();

    // Ativa o shader
    void use() const;

    // Função utilitária para enviar variáveis (uniforms) para a GPU
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif
