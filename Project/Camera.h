#pragma once
//#include <glm/glm.hpp>
#include <chrono>
#include "GP2Pipeline.h"

constexpr auto TO_DEGREES = (180.0f / 3.141f);
constexpr auto TO_RADIANS(3.141f / 180.0f);

class Camera
{
private:
    glm::mat4 CreateRotationX(float pitch)
    {
        glm::mat4 bufferMatrix{};
        bufferMatrix[0] = glm::vec4{ 1, 0, 0, 0 };
        bufferMatrix[1] = glm::vec4{ 0, cosf(pitch * TO_RADIANS), -sinf(pitch * TO_RADIANS), 0 };
        bufferMatrix[2] = glm::vec4{ 0, sinf(pitch * TO_RADIANS), cosf(pitch * TO_RADIANS), 0 };
        bufferMatrix[3] = glm::vec4{ 0, 0, 0, 1 };

        return bufferMatrix;
    }

    glm::mat4 CreateRotationY(float yaw)
    {
        glm::mat4 bufferMatrix{};
        bufferMatrix[0] = glm::vec4{ cosf(yaw * TO_RADIANS), 0, -sinf(yaw * TO_RADIANS), 0 };
        bufferMatrix[1] = glm::vec4{ 0, 1 , 0, 0 };
        bufferMatrix[2] = glm::vec4{ sinf(yaw * TO_RADIANS), 0, cosf(yaw * TO_RADIANS), 0 };
        bufferMatrix[3] = glm::vec4{ 0, 0, 0, 1 };

        return bufferMatrix;
    }

    glm::mat4 CreateRotationZ(float roll)
    {
        glm::mat4 bufferMatrix{};
        bufferMatrix[0] = glm::vec4{ cosf(roll * TO_RADIANS),  -sinf(roll * TO_RADIANS), 0 , 0 };
        bufferMatrix[1] = glm::vec4{ sinf(roll * TO_RADIANS), cosf(roll * TO_RADIANS) , 0, 0 };
        bufferMatrix[2] = glm::vec4{ 0, 0, 1 , 0 };
        bufferMatrix[3] = glm::vec4{ 0, 0, 0, 1 };

        return bufferMatrix;
    }

    glm::mat4 CreateRotation(float pitch, float yaw, float roll)
    {
        return CreateRotationX(pitch) * CreateRotationY(yaw) * CreateRotationZ(roll);
    }

public:
    Camera()
    {
        Camera({ 0.f, -2.f, 2.f }, 90.f);
    }

    Camera(const glm::vec3& _origin, float _fovAngle) :
        origin{ _origin },
        fovAngle{ _fovAngle }
    {
    }

    ~Camera() = default;

    const glm::vec3& GetOrigin() const { return origin; }

private:
    glm::vec3 origin{ 0.f, -2.f, 2.f };
    float fovAngle{ 90.f };

    glm::vec3 forward{ 0.f, 1.f, 0.f };
    glm::vec3 up{ 0.f, 0.f, 1.f };
    glm::vec3 right{ 1.f, 0.f, 0.f };

    float totalPitch{ 0.f };
    float totalYaw{ 90.f };

    glm::mat4 cameraToWorld{};

    std::chrono::steady_clock::time_point startTime{};
    glm::vec2 oldMousePos{0,0};
    bool firstUpdate{ true };
    float deltaT{};

public:
    glm::mat4 CalculateCameraToWorld()
    {

        ////1. calculate camera axis system
        //glm::vec3 cameraRight{ glm::normalize(glm::cross(forward, up)) };
        //glm::vec3 cameraUp{ glm::normalize(glm::cross(cameraRight, forward)) };

        //2. create cameraToWorld Matrix
        glm::mat4 cameraToWorldMatrix
        {
            glm::vec4{ right, 0},
            glm::vec4{ up, 0 },
            glm::vec4{ forward, 0 },
            glm::vec4{ origin, 1 }
        };

        ////3. update axis system to calculated system
        //right = cameraRight;
        //up = cameraUp;



        return cameraToWorldMatrix;
    }

    void KeyEvent(int key, int scancode, int action, int mods)
    {
        //std::cout << origin.x << ", " << origin.y << ", " << origin.z << "\n";
        //std::cout << deltaT << '\n';
        //implement movement
        const float movementSpeed{ 10.f * deltaT };
        if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {
            origin += forward * movementSpeed;
        }
        if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {
            origin -= forward * movementSpeed;
        }
        if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {
            origin -= right * movementSpeed;
        }
        if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {
            origin += right * movementSpeed;
        }
        if (key == GLFW_KEY_SPACE && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {
            origin += up * movementSpeed;
        }
        if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_REPEAT || action == GLFW_PRESS))
        {
            origin -= up * movementSpeed;
        }
    }

    void Update(GLFWwindow* window, glm::vec2 dragStart)
    {
        if (firstUpdate)
        {
            startTime = std::chrono::high_resolution_clock::now();
            firstUpdate = false;
        }
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        deltaT = deltaTime;
        startTime = currentTime;

        //std::cout << deltaT << '\n';

        //Mouse Input
        double mouseX{}, mouseY{};
        int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float changeX{ 0.f };
        float changeY{ 0.f };

        if (oldMousePos.x == 0 && oldMousePos.y == 0)
        {
            oldMousePos.x = static_cast<float>(mouseX);
            oldMousePos.y = static_cast<float>(mouseY);
        }
        else
        {
            changeX = static_cast<float>(mouseX) - oldMousePos.x;
            changeY = static_cast<float>(mouseY) - oldMousePos.y;
        }

        //implement rotation
        glm::mat4 rotationMatrix{};
        const float rotationSpeed{ 5.f * deltaTime };
        if (mouseState == GLFW_PRESS) //right mouse
        {
            //update members
            totalYaw += changeX * rotationSpeed;
            totalPitch += changeY * rotationSpeed;

            // Clamp pitch to avoid flipping
            if (totalPitch > 89.0f)
                totalPitch = 89.0f;
            if (totalPitch < -89.0f)
                totalPitch = -89.0f;

            glm::vec3 newFront;
            newFront.x = cos(glm::radians(totalYaw)) * cos(glm::radians(totalPitch));
            newFront.y = sin(glm::radians(totalYaw)) * cos(glm::radians(totalPitch));
            newFront.z = sin(glm::radians(totalPitch));
            forward = newFront;
            //forward = glm::normalize(newFront);

            right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f)));
            up = glm::normalize(glm::cross(right, forward));

            oldMousePos.x = static_cast<float>(mouseX);
            oldMousePos.y = static_cast<float>(mouseY);
        }
        else if(mouseState == GLFW_RELEASE)
        {
            oldMousePos.x = 0;
            oldMousePos.y = 0;
            return;
        }

        //std::cout << "Origin: " << origin.x << ", " << origin.y << ", " << origin.z << '\n';
        //std::cout << "Forward: " << forward.x << ", " << forward.y << ", " << forward.z << '\n';
        //std::cout << "Right: " << right.x << ", " << right.y << ", " << right.z << '\n';
        //std::cout << "Yaw: " << totalYaw << ", " << "Pitch: " << totalPitch << '\n';
        //std::cout << "DeltaT: " << deltaT << '\n';

    }
};