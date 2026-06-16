#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"

void usage() {
    printf("\n");
    printf("Uso:\n");
    printf("TAB                       Alterar objeto selecionado\n");
    printf("X, Y, Z                   Ativar/desativar rotação automática do objeto selecionado\n");
    printf("Clicar e arrastar         Rotacionar o objeto selecionado de forma livre\n");
    printf("Scroll do mouse           Zoom in/out (FOV)\n");
    printf("W, A, S, D                Movimentar a câmera (fly-by)\n");
    printf("P                         Alternar Projeção (Perspectiva/Ortográfica)\n");
    printf("M                         Alternar Modo de Visualização (Sólido/Wireframe)\n");
    printf("Setas                     Mover objeto nos eixos X e Z\n");
    printf("Espaço / Left Ctrl        Mover objeto no eixo Y (sobe/desce)\n");
    printf("I / O                     Aumentar/Diminuir escala do objeto\n");
    printf("Q ou ESC                  Encerrar\n");
    printf("\n");
}

class BezierCurve {
public:
    std::vector<glm::vec3> controlPoints;

    BezierCurve(const std::vector<glm::vec3>& points) : controlPoints(points) {}

    glm::vec3 evaluate(float t) {
        if (controlPoints.size() != 4) {
            // Suporta apenas Bézier Cúbica por enquanto
            return glm::vec3(0.0f); 
        }
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        glm::vec3 p = uuu * controlPoints[0]; // (1-t)^3 * P0
        p += 3 * uu * t * controlPoints[1];  // 3 * (1-t)^2 * t * P1
        p += 3 * u * tt * controlPoints[2];  // 3 * (1-t) * t^2 * P2
        p += ttt * controlPoints[3];         // t^3 * P3
        
        return p;
    }
};

struct SceneObject {
    Model* modelo;           // Ponteiro para a malha 3D
    glm::vec3 position;      // Posição no mundo (X, Y, Z)
    glm::vec3 rotation;      // Ângulos de Euler (X, Y, Z) em radianos
    glm::vec3 scale;         // Escala (X, Y, Z)
    bool rotateX = false, rotateY = false, rotateZ = false; // Rotações automáticas

    // Propriedades do Material
    glm::vec3 ka, kd, ks;
    float shininess;

    // Animação
    BezierCurve* animationCurve = nullptr;
    float animationTime = 0.0f;
    float animationDuration = 10.0f; // Duração em segundos para completar o percurso

    SceneObject(Model* mod, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) 
        : modelo(mod), position(pos), rotation(rot), scale(scl),
          ka(0.1f), kd(0.7f), ks(0.5f), shininess(32.0f) {}

    // Destrutor para limpar a memória da curva
    ~SceneObject();
};

// Implementação do destrutor do SceneObject após a definição de BezierCurve
SceneObject::~SceneObject() {
    delete animationCurve;
}

struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

// Função para carregar a cena a partir de um arquivo de configuração
void loadScene(
    const std::string& filename,
    Camera& camera,
    Light& light,
    std::vector<Model*>& models,
    std::vector<SceneObject>& scene
);

int main()
{
    Window appWindow(800, 600, "Trabalho GB");

    // Configurações iniciais que serão sobrescritas pelo arquivo de cena
    Camera camera(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0,1.0,0.0), -90.0, 0.0);
    Light light = {glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f)};
    bool perspective = true;

    Shader shader(
        "../src/GrauB/shaders/phong.vert",
        "../src/GrauB/shaders/phong.frag");

    glEnable(GL_DEPTH_TEST);

    // Carregamento de modelos e cena
    int modelSelected = 0;
    std::vector<Model*> model_pool; // Guarda os modelos para não duplicar na memória
    std::vector<SceneObject> scene;

    loadScene("../src/GrauB/scene.txt", camera, light, model_pool, scene);

    bool showWireframe = false;

    usage();

    // Loop da aplicação - "game loop"
    while (!appWindow.shouldClose()) {
        int width, height;
        glfwGetFramebufferSize(appWindow.get(), &width, &height);

        appWindow.clear(); 
        float dt = appWindow.getDeltaTime();

        // ------------------------------------------
        // ATUALIZAÇÃO DE ANIMAÇÕES
        // ------------------------------------------
        for (auto& object : scene) {
            if (object.animationCurve) {
                object.animationTime += dt;
            }

            // Rotação automática
            float rotSpeed = 1.0f * dt;
            if (object.rotateX) object.rotation.x += rotSpeed;
            if (object.rotateY) object.rotation.y += rotSpeed;
            if (object.rotateZ) object.rotation.z += rotSpeed;
        }

        // ------------------------------------------
        // PROCESSAMENTO DE INPUTS GERAIS
        // ------------------------------------------
        // Movimento da Câmera (WASD)
        if (appWindow.isKeyPressed(GLFW_KEY_W)) camera.moveForward(dt);
        if (appWindow.isKeyPressed(GLFW_KEY_S)) camera.moveBackward(dt);
        if (appWindow.isKeyPressed(GLFW_KEY_A)) camera.moveLeft(dt);
        if (appWindow.isKeyPressed(GLFW_KEY_D)) camera.moveRight(dt);

        // Zoom da Câmera (Scroll)
        float scrollY = appWindow.getScrollOffsetY();
        if (scrollY != 0.0f) camera.processMouseScroll(scrollY);

        // Modos de Visualização e Projeção
        if (appWindow.isKeyToggled(GLFW_KEY_M)) showWireframe = !showWireframe;
        if (appWindow.isKeyToggled(GLFW_KEY_P)) perspective = !perspective;

        // Seleção de Objeto (TAB)
        if (appWindow.isKeyToggled(GLFW_KEY_TAB)) {
            if (!scene.empty()) {
                modelSelected = (modelSelected + 1) % scene.size();
            }
        }

        // ------------------------------------------
        // PROCESSAMENTO DE INPUTS DO OBJETO ATIVO
        // ------------------------------------------
        // Rotação com o Mouse (Clique esquerdo + Arrastar)
        float offsetX = appWindow.getMouseOffsetX();
        float offsetY = appWindow.getMouseOffsetY();
        if (appWindow.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            if (!scene.empty()) {
                scene[modelSelected].rotation.y += offsetX * 0.01f;
                scene[modelSelected].rotation.x += offsetY * 0.01f; 
            }
        }

        // Translação do Objeto (Setas + Espaço/C)
        if (!scene.empty()) {
            float moveSpeed = 2.0f * dt;
            if (appWindow.isKeyPressed(GLFW_KEY_UP))    scene[modelSelected].position.z -= moveSpeed;
            if (appWindow.isKeyPressed(GLFW_KEY_DOWN))  scene[modelSelected].position.z += moveSpeed;
            if (appWindow.isKeyPressed(GLFW_KEY_LEFT))  scene[modelSelected].position.x -= moveSpeed;
            if (appWindow.isKeyPressed(GLFW_KEY_RIGHT)) scene[modelSelected].position.x += moveSpeed;
            if (appWindow.isKeyPressed(GLFW_KEY_SPACE)) scene[modelSelected].position.y += moveSpeed;
            if (appWindow.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) scene[modelSelected].position.y -= moveSpeed;
        }

        // Escala Uniforme (Teclas I e O)
        if (!scene.empty()) {
            float scaleSpeed = 1.0f * dt;
            if (appWindow.isKeyPressed(GLFW_KEY_O)) scene[modelSelected].scale += glm::vec3(scaleSpeed);
            if (appWindow.isKeyPressed(GLFW_KEY_I)) scene[modelSelected].scale -= glm::vec3(scaleSpeed);
            if (scene[modelSelected].scale.x < 0.1f) scene[modelSelected].scale = glm::vec3(0.1f); 
        }

        // Rotação contínua nos Eixos (Teclas X, Y, Z)
        if (!scene.empty()) {
            if (appWindow.isKeyToggled(GLFW_KEY_X)) scene[modelSelected].rotateX = !scene[modelSelected].rotateX;
            if (appWindow.isKeyToggled(GLFW_KEY_Y)) scene[modelSelected].rotateY = !scene[modelSelected].rotateY;
            if (appWindow.isKeyToggled(GLFW_KEY_Z)) scene[modelSelected].rotateZ = !scene[modelSelected].rotateZ;
        }

        // ------------------------------------------
        // PREPARAÇÃO DO RENDER (Shaders e Matrizes Globais)
        // ------------------------------------------
        shader.use();

        // Configuração da Projeção (Perspectiva ou Ortográfica com P)
        glm::mat4 projection;
        if (perspective) {
            projection = glm::perspective(glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 100.0f);
        } else {
            float aspect = (float)width / (float)height;
            projection = glm::ortho(-3.0f * aspect, 3.0f * aspect, -3.0f, 3.0f, 0.1f, 100.0f);
        }
        shader.setMat4("projection", projection);

        // Matriz de Visualização (Câmera)
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("view", view);

        // Iluminação Global
        shader.setVec3("light.position", light.position); 
        shader.setVec3("light.color", light.color); 
        shader.setVec3("viewPos", camera.position);

        // ------------------------------------------
        // DESENHAR A CENA
        // ------------------------------------------
        for (int i = 0; i < scene.size(); i++) {
            
            // Aplica a transformação deste objeto
            glm::mat4 modelMatrix = glm::mat4(1.0f);

            // Calcula a posição final: Posição Base + Deslocamento da Curva
            glm::vec3 finalPos = scene[i].position;
            if (scene[i].animationCurve) {
                float t = fmod(scene[i].animationTime, scene[i].animationDuration) / scene[i].animationDuration;
                finalPos += scene[i].animationCurve->evaluate(t);
            }

            modelMatrix = glm::translate(modelMatrix, finalPos);
            
            // Ordem: Yaw (Y), Pitch (X), Roll (Z)
            modelMatrix = glm::rotate(modelMatrix, scene[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, scene[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, scene[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            
            modelMatrix = glm::scale(modelMatrix, scene[i].scale);
            shader.setMat4("model", modelMatrix);

            bool isSelected = (i == modelSelected);

            if (showWireframe) {
                // Desliga a iluminação para ver a malha
                shader.setVec3("light.color", glm::vec3(0.0f, 0.0f, 0.0f));
            } else {
                // Modo Realista com materiais
                shader.setVec3("light.color", light.color);
                
                // Destaca o objeto selecionado aumentando sua componente ambiente
                glm::vec3 final_ka = isSelected ? scene[i].ka * 2.5f : scene[i].ka;

                shader.setVec3("material.ka", final_ka);
                shader.setVec3("material.kd", scene[i].kd);
                shader.setVec3("material.ks", scene[i].ks);
                shader.setFloat("material.shininess", scene[i].shininess);
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(3.0f, 3.0f); 
            
            scene[i].modelo->draw(); 
            
            glDisable(GL_POLYGON_OFFSET_FILL);

            if (showWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
                
                if (isSelected) {
                    shader.setVec3("material.ka", glm::vec3(1.0f, 1.0f, 0.0f)); 
                } else {
                    shader.setVec3("material.ka", glm::vec3(0.0f, 1.0f, 0.0f)); 
                }
                shader.setVec3("material.kd", glm::vec3(0.0f));
                shader.setVec3("material.ks", glm::vec3(0.0f));
                shader.setVec3("light.color", glm::vec3(1.0f, 1.0f, 1.0f));
                glDepthFunc(GL_LEQUAL); 
                
                scene[i].modelo->draw(); 
                
                glDepthFunc(GL_LESS); 
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
            }
        }

        appWindow.swapBuffersAndPoll();
    }

    // Limpeza
    for (auto model : model_pool) {
        delete model;
    }

    return 0;
}


void loadScene(
    const std::string& filename,
    Camera& camera,
    Light& light,
    std::vector<Model*>& models,
    std::vector<SceneObject>& scene)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de cena '" << filename << "'" << std::endl;
        return;
    }

    std::string line;
    SceneObject* currentObject = nullptr;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "camera_pos") { // Ex: camera_pos 0 5 20
            ss >> camera.position.x >> camera.position.y >> camera.position.z;
        } else if (command == "camera_yaw_pitch") { // Ex: camera_yaw_pitch -90 -10
            ss >> camera.yaw >> camera.pitch;
            camera.updateCameraVectors();
        } else if (command == "camera_fov") { // Ex: camera_fov 45
            ss >> camera.zoom;
        } else if (command == "light_pos") { // Ex: light_pos 0 20 0
            ss >> light.position.x >> light.position.y >> light.position.z;
        } else if (command == "light_color") { // Ex: light_color 1 1 1
            ss >> light.color.r >> light.color.g >> light.color.b;
        } else if (command == "model") { // Ex: model ../path/to/model.obj
            std::string path;
            // Pega o resto da linha para suportar caminhos com espaços
            getline(ss, path);
            // Remove espaços em branco no início e no fim
            path.erase(0, path.find_first_not_of(" \t"));
            path.erase(path.find_last_not_of(" \t") + 1);

            if (path.empty()) {
                std::cerr << "Erro na linha " << lineNum << ": Comando 'model' sem um caminho de arquivo." << std::endl;
                continue;
            }

            models.push_back(new Model(path));
            scene.emplace_back(models.back(), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
            currentObject = &scene.back();
        } else if (currentObject) {
            if (command == "pos") { // Ex: pos 0 -2 0
                ss >> currentObject->position.x >> currentObject->position.y >> currentObject->position.z;
            } else if (command == "rot") { // Ex: rot 0 45 0
                ss >> currentObject->rotation.x >> currentObject->rotation.y >> currentObject->rotation.z;
                currentObject->rotation = glm::radians(currentObject->rotation);
            } else if (command == "scale") { // Ex: scale 20 0.1 20
                ss >> currentObject->scale.x >> currentObject->scale.y >> currentObject->scale.z;
            } else if (command == "material") { // Ex: material 0.1 0.1 0.1 0.5 0.5 0.5 0.1 0.1 0.1 16.0
                ss >> currentObject->ka.r >> currentObject->ka.g >> currentObject->ka.b;
                ss >> currentObject->kd.r >> currentObject->kd.g >> currentObject->kd.b;
                ss >> currentObject->ks.r >> currentObject->ks.g >> currentObject->ks.b;
                ss >> currentObject->shininess;
            } else if (command == "animate_bezier") { // Ex: animate_bezier 15.0
                std::vector<glm::vec3> points(4);
                bool success = true;
                for (int i = 0; i < 4; ++i) {
                    if (!getline(file, line)) {
                        std::cerr << "Erro: Fim de arquivo inesperado ao ler pontos de controle da curva de Bezier." << std::endl;
                        success = false;
                        break;
                    }
                    lineNum++;
                    std::stringstream p_ss(line);
                    std::string p_cmd;
                    if (!(p_ss >> p_cmd >> points[i].x >> points[i].y >> points[i].z)) {
                        std::cerr << "Erro na linha " << lineNum << ": Linha de ponto de controle mal formada: '" << line << "'" << std::endl;
                        success = false;
                        break;
                    }
                }
                if (success) {
                    currentObject->animationCurve = new BezierCurve(points);
                    ss >> currentObject->animationDuration; // Lê a duração da linha 'animate_bezier'
                }
            }
        }
    }

    file.close();
    std::cout << "Cena '" << filename << "' carregada com " << scene.size() << " objetos." << std::endl;
}
