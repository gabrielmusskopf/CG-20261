#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Variáveis que vão para o Fragment Shader
out vec3 FragPos;
out vec3 Normal;
out vec3 ObjectColor;

void main() {
    // Calcula a posição do vértice no mundo 3D
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Calcula a normal corrigindo possíveis distorções de escala do objeto
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Repassa a cor base
    ObjectColor = aColor; 
    
    // Calcula a posição final
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
