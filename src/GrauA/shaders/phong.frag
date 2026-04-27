#version 450
in vec3 FragPos;
in vec3 Normal;
in vec3 ObjectColor;

out vec4 FragColor;

// Posições e Cores da Luz/Câmara
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

// Propriedades do Material
uniform float ka; // Coeficiente Ambiente
uniform float kd; // Coeficiente Difuso
uniform float ks; // Coeficiente Especular
uniform float shininess; // Brilho

void main() {
    // Luz Ambiente
    vec3 ambient = ka * lightColor;

    // Luz Difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // Produto escalar (ângulo entre a luz e a face)
    vec3 diffuse = kd * diff * lightColor;

    // Luz Especular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = ks * spec * lightColor;

    // Combina tudo e multiplica pela cor do objeto
    vec3 result = (ambient + diffuse + specular) * ObjectColor;
    FragColor = vec4(result, 1.0);
}
