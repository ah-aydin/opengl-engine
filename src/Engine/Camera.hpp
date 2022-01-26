#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace oe
{
    class Camera
    {
    public:
        Camera();
        Camera(glm::vec3 startPosition, float pitch, float yaw, float roll, float fov, float aspectRatio);
        Camera(float px, float py, float pz, float pitch, float yaw, float roll, float fov, float aspectRatio);
        
        float fov;
        float aspectRatio = 800.0f / 600.0f;
        float sensitivity = 0.07f;
        float moveSpeed = 5.f;
        float pitch = 0, yaw = 0, roll = 0;
        glm::vec3 position;

        glm::mat4 getViewMatrix() { return viewMatrix; }
        glm::mat4 getProjectionMatrix() { return projectionMat; };
        glm::vec3 getPosition() { return position; }
        glm::vec3 getForward() { return forward; }

        // Input methods
        void handleInput(float iForward, float iRight, float iUp, float iDown, float mouseX, float mouseY);
    private:
        glm::mat4 projectionMat;
        glm::vec3 worldUp;
        glm::vec3 forward, right, up, target;
        void calculateVectors();
        glm::mat4 viewMatrix;
    };
}