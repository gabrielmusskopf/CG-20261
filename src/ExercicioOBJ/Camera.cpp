#include "Camera.h"

Camera::Camera(glm::vec3 startPos, glm::vec3 startUp, float startYaw, float startPitch) :
front(glm::vec3(0.0,0.0,1.0)), movementSpeed(2.5f), mouseSensitivity(0.1f), zoom(45.0f)
{
    position = startPos;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::moveForward(float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    position += front * velocity;
}

void Camera::moveBackward(float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    position -= front * velocity;
}
void Camera::moveLeft(float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    position -= right * velocity;
}
void Camera::moveRight(float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    // 1. Aplica a sensibilidade do mouse para o movimento não ser bizarramente rápido
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    // 2. Atualiza os ângulos de rotação da câmera
    yaw   += xoffset;
    pitch += yoffset;

    // 3. Trava o eixo Y (Pitch) se solicitado
    // Isso impede que olhe "tão para cima" que a câmera vire de cabeça para baixo
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // 4. Usa a trigonometria já implementada para recalcular os vetores Front, Right e Up
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    zoom -= (float)yoffset * 2.0f; // Multiplicador para a velocidade do zoom
    
    // Limites de segurança para o Campo de Visão (FOV)
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front,worldUp));
    up = glm::normalize(glm::cross(right,front));
}
