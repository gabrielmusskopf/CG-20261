#version 450

in vec3 FragPos;
in vec3 Normal;
in vec3 ObjectColor; // Opcional, mantido para compatibilidade com o vertex shader

out vec4 FragColor;

struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // Luz Ambiente
    vec3 ambient = material.ka * light.color;

    // Luz Difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = material.kd * diff * light.color;

    // Luz Especular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.ks * spec * light.color;

    // Combina tudo (usando propriedades de material em vez do ObjectColor)
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}