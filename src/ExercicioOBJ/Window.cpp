#include "Window.h"
#include "GLFW/glfw3.h"
#include <iostream>

Window::Window(int width, int height, const char* title) : deltaTime(0.0f), lastFrame(0.0f) {
    // Inicializa o array de teclas
    for (int i = 0; i < 1024; i++) {
        keys[i] = false;
        keysProcessed[i] = false;
    }

    glfwInit();

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cout << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    // Associar a instância desta classe à janela GLFW
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    // Esconde e prende o cursor à janela (essencial para movimento livre)
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetScrollCallback(window, scroll_callback);
    scrollOffsetY = 0.0f;

    // Valores iniciais para o mouse
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouse = true;
    mouseOffsetX = 0.0f;
    mouseOffsetY = 0.0f;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    glViewport(0, 0, frameWidth, frameHeight);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::clear() const {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapBuffersAndPoll() {
    glfwSwapBuffers(window);
    glfwPollEvents();

    // Atualiza o tempo automaticamente
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

bool Window::isKeyPressed(int key) const {
    if (key >= 0 && key < 1024) return keys[key];
    return false;
}

bool Window::isKeyToggled(int key) {
    if (key >= 0 && key < 1024) {
        if (keys[key] && !keysProcessed[key]) {
            keysProcessed[key] = true;
            return true; // Acabou de ser pressionada
        }
    }
    return false;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // Recupera o ponteiro da nossa instância de Window
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q ) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            win->keys[key] = true;
            win->keysProcessed[key] = false;
        } else if (action == GLFW_RELEASE) {
            win->keys[key] = false;
            win->keysProcessed[key] = false;
        }
    }
}

// Devolve o deslocamento e repõe a zero (para não continuar a rodar se o mouse parar)
float Window::getMouseOffsetX() {
    float offset = mouseOffsetX;
    mouseOffsetX = 0.0f; 
    return offset;
}

float Window::getMouseOffsetY() {
    float offset = mouseOffsetY;
    mouseOffsetY = 0.0f;
    return offset;
}

// A função que o GLFW chama sempre que o mouse se move
void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // Evita um salto brusco ("jump") na câmara quando o mouse entra no ecrã pela primeira vez
    if (win->firstMouse) {
        win->lastX = xpos;
        win->lastY = ypos;
        win->firstMouse = false;
    }

    // Calcula o deslocamento
    win->mouseOffsetX = xpos - win->lastX;
    win->mouseOffsetY = win->lastY - ypos; // Invertido, porque o eixo Y do ecrã cresce de cima para baixo

    // Atualiza a última posição
    win->lastX = xpos;
    win->lastY = ypos;
}

bool Window::isMouseButtonPressed(int button) const {
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}
// Retorna o movimento do scroll e o zera
float Window::getScrollOffsetY() {
    float offset = scrollOffsetY;
    scrollOffsetY = 0.0f;
    return offset;
}

// A função que o GLFW chama quando a bolinha do mouse gira
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    // Somar para o caso do usuário girar a roda muito rápido em um único frame
    win->scrollOffsetY += static_cast<float>(yoffset); 
}
