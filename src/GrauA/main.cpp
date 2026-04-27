#include <cstdio>
#include <string>
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

using namespace std;

void usage() {
    printf("\n");
    printf("Uso:\n");
    printf("TAB                       Alterar objeto selecionado\n");
    printf("X, Y e Z                  Rotacionar objeto no eixo correspondente\n");
    printf("Clicar e arrastar         Rotacionar o objeto de forma livre\n");
    printf("Scroll do mouse           Zoom in/out\n");
    printf("W, A, S, D                Movimentar a câmera\n");
    printf("P                         Perspectiva\n");
    printf("M                         Visualizar malha\n");
    printf("Setas, Espaco e C         Movimentar o objeto\n");
    printf("Q ou ESC                  Encerrar\n");
    printf("\n");
}

struct SceneObject {
    Model* modelo;           // Ponteiro para a malha 3D
    glm::vec3 position;      // Posição no mundo (X, Y, Z)
    glm::vec3 rotation;      // Ângulos de Euler (X, Y, Z) em radianos
    glm::vec3 scale;         // Escala (X, Y, Z)
    bool rotateX = false, rotateY = false, rotateZ = false; // Rotações dos eixos

    SceneObject(Model* mod, glm::vec3 pos) 
        : modelo(mod), position(pos), rotation(0.0f), scale(1.0f) {}
};

int main()
{
    Window appWindow(600, 600, "Trabalho GA");

    Camera camera(glm::vec3(0.0, 0.0, -3.0), glm::vec3(0.0,1.0,0.0),90.0,0.0);
    bool rotateX = false, rotateY = false, rotateZ = false;
    bool perspective = true;

    Shader shader(
            "../src/GrauA/shaders/phong.vert",
            "../src/GrauA/shaders/phong.frag");

    glEnable(GL_DEPTH_TEST);

    int modelSelected = 0;
    Model suzanne = {"../assets/Modelos3D/SuzanneSubdiv1.obj"};
    Model cube = {"../assets/Modelos3D/Cube.obj"};
    Model octaedro = {"../assets/Modelos3D/Octaedro.obj"};
    Model piramid = {"../assets/Modelos3D/Piramide.obj"};

    std::vector<SceneObject> scene;
    scene.push_back(SceneObject(&piramid, glm::vec3(5.0f, -1.0f, 0.0f)));
    scene.push_back(SceneObject(&cube, glm::vec3(1.5f, 0.0f, 0.0f)));
    scene.push_back(SceneObject(&suzanne, glm::vec3(-1.5f, 0.0f, 0.0f)));
    scene.push_back(SceneObject(&octaedro, glm::vec3(-5.0f, 0.0f, 0.0f)));

    bool showWireframe = false;

    usage();

    // Loop da aplicação - "game loop"
    while (!appWindow.shouldClose()) {
        appWindow.clear(); 
        float dt = appWindow.getDeltaTime();

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
        if (appWindow.isKeyToggled(GLFW_KEY_P)) perspective = !perspective; // Alterna a Projeção

        // Seleção de Objeto (TAB)
        if (appWindow.isKeyToggled(GLFW_KEY_TAB)) {
            modelSelected = (modelSelected + 1) % scene.size();
        }

        // ------------------------------------------
        // PROCESSAMENTO DE INPUTS DO OBJETO ATIVO
        // ------------------------------------------
        // Rotação com o Mouse (Clique esquerdo + Arrastar)
        float offsetX = appWindow.getMouseOffsetX();
        float offsetY = appWindow.getMouseOffsetY();
        if (appWindow.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            scene[modelSelected].rotation.y += offsetX * 0.05f;
            scene[modelSelected].rotation.x -= offsetY * 0.05f; 
        }

        // Translação do Objeto (Setas + Espaço/C)
        float moveSpeed = 2.0f * dt;
        if (appWindow.isKeyPressed(GLFW_KEY_UP))    scene[modelSelected].position.y -= moveSpeed;
        if (appWindow.isKeyPressed(GLFW_KEY_DOWN))  scene[modelSelected].position.y += moveSpeed;
        if (appWindow.isKeyPressed(GLFW_KEY_LEFT))  scene[modelSelected].position.x += moveSpeed;
        if (appWindow.isKeyPressed(GLFW_KEY_RIGHT)) scene[modelSelected].position.x -= moveSpeed;
        if (appWindow.isKeyPressed(GLFW_KEY_SPACE)) scene[modelSelected].position.z += moveSpeed; // Tras
        if (appWindow.isKeyPressed(GLFW_KEY_C))     scene[modelSelected].position.z -= moveSpeed; // Frente

        // Escala Uniforme (Teclas I e O)
        float scaleSpeed = 1.0f * dt;
        if (appWindow.isKeyPressed(GLFW_KEY_I)) scene[modelSelected].scale -= glm::vec3(scaleSpeed); // Diminui
        if (appWindow.isKeyPressed(GLFW_KEY_O)) scene[modelSelected].scale += glm::vec3(scaleSpeed); // Aumenta
        if (scene[modelSelected].scale.x < 0.1f) scene[modelSelected].scale = glm::vec3(0.1f); 

        // Rotação contínua nos Eixos (Teclas X, Y, Z)
        if (appWindow.isKeyPressed(GLFW_KEY_X)) {
            scene[modelSelected].rotateX = true;
            scene[modelSelected].rotateY = false;
            scene[modelSelected].rotateZ = false;
        }
        if (appWindow.isKeyPressed(GLFW_KEY_Y)) {
            scene[modelSelected].rotateX = false;
            scene[modelSelected].rotateY = true;
            scene[modelSelected].rotateZ = false;
        }
        if (appWindow.isKeyPressed(GLFW_KEY_Z)) {
            scene[modelSelected].rotateX = false;
            scene[modelSelected].rotateY = false;
            scene[modelSelected].rotateZ = true;
        }

        float rotSpeed = 2.0f * dt; // Velocidade da rotação
        for (int i = 0; i < scene.size(); i++) {
            // Se o toggle estiver ativo, somamos um pouquinho ao ângulo atual
            if (scene[i].rotateX) scene[i].rotation.x += rotSpeed;
            if (scene[i].rotateY) scene[i].rotation.y += rotSpeed;
            if (scene[i].rotateZ) scene[i].rotation.z += rotSpeed;
        }

        // ------------------------------------------
        // PREPARAÇÃO DO RENDER (Shaders e Matrizes Globais)
        // ------------------------------------------
        shader.use();

        // Configuração da Projeção (Perspectiva ou Ortográfica com P)
        glm::mat4 projection;
        if (perspective) {
            projection = glm::perspective(glm::radians(camera.zoom), 800.0f/600.0f, 0.1f, 100.0f);
        } else {
            projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 100.0f);
        }
        shader.setMat4("projection", projection);

        // Matriz de Visualização (Câmera)
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("view", view);

        // Iluminação Global
        shader.setVec3("lightPos", glm::vec3(0.0f, 2.0f, 3.0f)); 
        shader.setVec3("viewPos", camera.position);

        // ------------------------------------------
        // DESENHAR A CENA
        // ------------------------------------------
        for (int i = 0; i < scene.size(); i++) {
            
            // Aplica a transformação deste objeto
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, scene[i].position);
            
            // Ordem: Yaw (Y), Pitch (X), Roll (Z)
            modelMatrix = glm::rotate(modelMatrix, scene[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, scene[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, scene[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            
            modelMatrix = glm::scale(modelMatrix, scene[i].scale);
            shader.setMat4("model", modelMatrix);

            bool isSelected = (i == modelSelected);

            if (showWireframe) {
                // Modo Escuro
                shader.setVec3("lightColor", glm::vec3(0.0f, 0.0f, 0.0f));
                shader.setFloat("ka", 0.0f); 
                shader.setFloat("kd", 0.0f); 
                shader.setFloat("ks", 0.0f);
                shader.setFloat("shininess", 1.0f); 
            } else {
                // Modo Realista
                shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
                
                if (isSelected) {
                    shader.setFloat("ka", 0.4f); // Destaca o selecionado
                } else {
                    shader.setFloat("ka", 0.1f);
                }
                
                shader.setFloat("kd", 0.7f); 
                shader.setFloat("ks", 0.5f);
                shader.setFloat("shininess", 32.0f); 
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(3.0f, 3.0f); 
            
            scene[i].modelo->draw(); 
            
            glDisable(GL_POLYGON_OFFSET_FILL);

            if (showWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
                
                if (isSelected) {
                    shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.0f)); 
                } else {
                    shader.setVec3("lightColor", glm::vec3(0.0f, 1.0f, 0.0f)); 
                }
                
                shader.setFloat("ka", 1.0f); 
                shader.setFloat("kd", 0.0f);
                shader.setFloat("ks", 0.0f);
                shader.setFloat("shininess", 1.0f); 
                
                glDepthFunc(GL_LEQUAL); 
                
                scene[i].modelo->draw(); 
                
                glDepthFunc(GL_LESS); 
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
            }
        }

        appWindow.swapBuffersAndPoll();
    }

    return 0;
}
