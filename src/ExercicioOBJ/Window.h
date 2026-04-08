#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {

public:
    // O construtor inicia o GLFW, cria a janela e carrega o GLAD
    Window(int width, int height, const char* title);
    // O destrutor limpa o GLFW
    ~Window();

    bool shouldClose() const;
    void clear() const;
    void swapBuffersAndPoll();

    // Métodos para checar os inputs sem precisar de variáveis globais
    bool isKeyPressed(int key) const;
    bool isKeyToggled(int key); // Útil para teclas que alternam estados (ex: P para perspectiva)

    float getDeltaTime() const { return deltaTime; }
    GLFWwindow* get() const { return window; } // Caso precise do ponteiro original

    float getMouseOffsetX();
    float getMouseOffsetY();
    bool isMouseButtonPressed(int button) const;

    float getScrollOffsetY();

private:
    GLFWwindow* window;
    float deltaTime;
    float lastFrame;

    // Arrays para guardar o estado de cada tecla
    bool keys[1024];
    bool keysProcessed[1024];

    float mouseOffsetX;
    float mouseOffsetY;
    float lastX;
    float lastY;
    bool firstMouse;

    float scrollOffsetY;

    // A função de callback estática obrigatória do GLFW
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
