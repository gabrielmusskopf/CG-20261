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
    printf("Setas direita e esquerda  Alterar objeto\n");
    printf("1, 2, 3, 4, 5, 6          Pressionar e manter para trocar a posição da câmera\n");
    printf("X, Y e Z                  Rotacionar objeto no eixo correspondente\n");
    printf("W, A, S, D                Movimentar a câmera\n");
    printf("P                         Perspectiva\n");
    printf("Q ou ESC                  Encerrar\n");
    printf("\n");
}

int main()
{
    Window appWindow(600, 600, "Trabalho OBJ");

    Camera camera(glm::vec3(0.0, 0.0, -3.0), glm::vec3(0.0,1.0,0.0),90.0,0.0);
    bool rotateX = false, rotateY = false, rotateZ = false;
    bool perspective = true;

    Shader shader(
            "../src/ExercicioOBJ/shaders/hello.vert",
            "../src/ExercicioOBJ/shaders/hello.frag");

    glEnable(GL_DEPTH_TEST);

    int modelSelected = 0;
    Model models[] = {
        {"../assets/Modelos3D/SuzanneSubdiv1.obj"},
        {"../assets/Modelos3D/Cube.obj"}, 
        {"../assets/Modelos3D/Octaedro.obj"}, 
        {"../assets/Modelos3D/Piramide.obj"}};

    float modelAngleX = 0.0f; // Rotação no eixo X (Cima/Baixo)
    float modelAngleY = 0.0f; // Rotação no eixo Y (Esquerda/Direita)

    usage();

    // Loop da aplicação - "game loop"
    while (!appWindow.shouldClose()) {
        appWindow.clear();

        // --- CONTROLE DE INPUTS ---
        float dt = appWindow.getDeltaTime();
        if (appWindow.isKeyPressed(GLFW_KEY_W)) camera.moveForward(dt);
        if (appWindow.isKeyPressed(GLFW_KEY_S)) camera.moveBackward(dt);
        if (appWindow.isKeyPressed(GLFW_KEY_A)) camera.moveLeft(dt);
        if (appWindow.isKeyPressed(GLFW_KEY_D)) camera.moveRight(dt);

        if (appWindow.isKeyToggled(GLFW_KEY_P)) perspective = !perspective;
        
        if (appWindow.isKeyToggled(GLFW_KEY_X)) { rotateX = true; rotateY = false; rotateZ = false; }
        if (appWindow.isKeyToggled(GLFW_KEY_Y)) { rotateX = false; rotateY = true; rotateZ = false; }
        if (appWindow.isKeyToggled(GLFW_KEY_Z)) { rotateX = false; rotateY = false; rotateZ = true; }

        if (appWindow.isKeyToggled(GLFW_KEY_RIGHT)) { modelSelected = ++modelSelected % size(models); }
        if (appWindow.isKeyToggled(GLFW_KEY_LEFT)) { modelSelected = --modelSelected % size(models); }

        // --- CONTROLE DO MOUSE --- 
        float offsetX = appWindow.getMouseOffsetX();
        float offsetY = appWindow.getMouseOffsetY();

        // Só gira o objeto se o botão esquerdo (0) estiver pressionado
        if (appWindow.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            float sensibilidade = 0.025f;

            // O movimento horizontal do mouse afeta o eixo Y (gira para os lados)
            modelAngleY += offsetX * sensibilidade;
            // O movimento vertical do mouse afeta o eixo X (gira para cima/baixo)
            modelAngleX -= offsetY * sensibilidade; 
        }

        float scrollY = appWindow.getScrollOffsetY();
        if (scrollY != 0.0f) {
            camera.processMouseScroll(scrollY);
        }

        // --- MATRIZES E DESENHO ---
        shader.use();

        glm::mat4 projection;
        if (perspective) { projection = glm::perspective(glm::radians(camera.zoom), 600.0f/600.0f, 0.1f, 100.0f); }
        else { projection = glm::ortho(-3.0, 3.0, -3.0, 3.0, 0.1, 100.0); }
        shader.setMat4("projection", projection);

        // Define as views fixas baseadas no input numérico
        glm::mat4 view = camera.getViewMatrix();
        if (appWindow.isKeyPressed(GLFW_KEY_1)) view = glm::lookAt(glm::vec3(0,0,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));
        if (appWindow.isKeyPressed(GLFW_KEY_2)) view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
        if (appWindow.isKeyPressed(GLFW_KEY_3)) view = glm::lookAt(glm::vec3(-3,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
        if (appWindow.isKeyPressed(GLFW_KEY_4)) view = glm::lookAt(glm::vec3(3,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
        if (appWindow.isKeyPressed(GLFW_KEY_5)) view = glm::lookAt(glm::vec3(0,3,0), glm::vec3(0,0,0), glm::vec3(0,0,1));
        if (appWindow.isKeyPressed(GLFW_KEY_6)) view = glm::lookAt(glm::vec3(0,-3,0), glm::vec3(0,0,0), glm::vec3(0,0,1));
        shader.setMat4("view", view);

        // Modelo e Rotação
        glm::mat4 model = glm::mat4(1);
        // Aplica a rotação do mouse
        model = glm::rotate(model, modelAngleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotação Y
        model = glm::rotate(model, modelAngleX, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotação X

        // Mantém a rotação automática se as teclas X, Y, Z estiverem ativas
        float angle = (GLfloat)glfwGetTime();
        if (rotateX) model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        if (rotateY) model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        if (rotateZ) model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setMat4("model", model);

        // Draw Call
        models[modelSelected].draw();

        // Troca buffers e computa o delta time
        appWindow.swapBuffersAndPoll();
    }

    return 0;
}
