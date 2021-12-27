#include "Camera.hpp"

#include "Time.hpp"

namespace oe
{
    Camera::Camera() : 
        pitch(0), yaw(0), roll(0),
        projectionMat(glm::perspective(glm::radians(75.0f), 800.0f / 600.0f, 0.1f, 100.0f))
    {
        this->fov = 75;
        this->aspectRatio = 800.0f / 600.0f;
        position = glm::vec3(0);
        worldUp = glm::vec3(0, 1, 0);
        calculateVectors();
    }

    Camera::Camera(glm::vec3 startPosition, float pitch, float yaw, float roll, float fov, float aspectRatio) : 
        pitch(pitch), yaw(yaw), roll(roll),
        projectionMat(glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f))
    {
        this->fov = fov;
        this->aspectRatio = aspectRatio;
        position = startPosition;
        worldUp = glm::vec3(0, 1, 0);
        calculateVectors();
    }

    Camera::Camera(float px, float py, float pz, float pitch, float yaw, float roll, float fov, float aspectRatio) : 
        pitch(pitch), yaw(yaw), roll(roll),
        projectionMat(glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f))
    {
        this->fov = fov;
        this->aspectRatio = aspectRatio;
        position = glm::vec3(px, py, pz);
        worldUp = glm::vec3(0, 1, 0);
        calculateVectors();
    }

    void Camera::handleInput(float iForward, float iRight, float iUp, float iDown, float mouseX, float mouseY)
    {
        float verticalMove = iUp - iDown;
        position += iForward * moveSpeed * forward * Time::getDeltaTime() + 
                    iRight * moveSpeed * right * Time::getDeltaTime() + 
                    verticalMove * moveSpeed * worldUp * Time::getDeltaTime();
        yaw += mouseX * sensitivity;
        pitch += mouseY * sensitivity;
        // Clamp vertical look
        if (pitch > 89) pitch = 89;
        if (pitch < -89) pitch = -89;

        calculateVectors();
    }

    void Camera::calculateVectors()
    {
        // Calculate direction vector based on rotation
        glm::vec3 direction = glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        );

        // Calculate 3 directional vectors
        forward = glm::normalize(direction);
        right = glm::normalize(
            glm::cross(forward, worldUp)
        );
        up = glm::normalize(
            glm::cross(right, forward)
        );
        target = forward + position;
    }

    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(position, target, worldUp);
    }
}