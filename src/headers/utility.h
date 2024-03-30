#ifndef UTILITY_H
#define UTILITY_H


#include <string>
#include <stdlib.h>
#include <unordered_map>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Camera.h"

namespace utility
{
    void LogVec3(glm::vec3 vec3)
    {
        std::cout << "x: " << vec3.x << " y: " << vec3.y << " z: " << vec3.z << "\n";
    }

    enum ControllerType
    {
        PS4,
        Joystick
    };

    void GLAPIENTRY
        MessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
    {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
    }

    int InitGLFW(GLFWwindow*& window, GLFWmonitor*& monitor, GLFWvidmode*& mode, const std::string& window_name)
    {
        int return_value = 0;
        if (!glfwInit())
        {
            return_value = -1;
        }

        monitor = glfwGetPrimaryMonitor();
        mode = (GLFWvidmode*)glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window = glfwCreateWindow(mode->width, mode->height, window_name.c_str(), nullptr, nullptr);
            
        if (window == nullptr)
        {
            glfwTerminate();
            return_value = -1;
        }

        glfwMakeContextCurrent(window);
        return return_value;
    }

    inline int InitGlad()
    {
        int return_value = 0;

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            return -1;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
        return return_value;
    }

    void DebugCameraMovement(GLFWwindow* window, std::shared_ptr <llr::Camera> camera, float delta_time)
    {
        static float move_speed = 1.0f;
        static float look_speed = 100.0f;

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            camera->set_position(camera->get_position() + camera->get_front() * move_speed * delta_time);
            camera->UpdateDirectionVectors();
        }

        if (glfwGetKey(window, GLFW_KEY_S))
        {
            camera->set_position(camera->get_position() - camera->get_front() * move_speed * delta_time);
            camera->UpdateDirectionVectors();
        }

        if (glfwGetKey(window, GLFW_KEY_A))
        {
            camera->set_position(camera->get_position() - camera->get_right() * move_speed * delta_time);
            camera->UpdateDirectionVectors();
        }

        if (glfwGetKey(window, GLFW_KEY_D))
        {
            camera->set_position(camera->get_position() + camera->get_right() * move_speed * delta_time);
            camera->UpdateDirectionVectors();
        }


        if (glfwGetKey(window, GLFW_KEY_UP))
        {
            camera->set_pitch(camera->get_pitch() + look_speed * delta_time);
            camera->UpdateDirectionVectors();
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN))
        {
            camera->set_pitch(camera->get_pitch() - look_speed * delta_time);
            camera->UpdateDirectionVectors();
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT))
        {
            camera->set_yaw(camera->get_yaw() + look_speed * delta_time);
            camera->UpdateDirectionVectors();
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT))
        {
            camera->set_yaw(camera->get_yaw() - look_speed * delta_time);
            camera->UpdateDirectionVectors();
        }

        if (camera->get_pitch() > 89.0f)
        {
            camera->set_pitch(89.0f);
            camera->UpdateDirectionVectors();
        }

        if (camera->get_pitch() < -89.0f)
        {
            camera->set_pitch(-89.0f);
            camera->UpdateDirectionVectors();
        }
    }
    
    void DebugCameraMovementJoystick(GLFWwindow* window, std::shared_ptr <llr::Camera> camera, float delta_time)
    {
        static float move_speed = 1.0f;
        static float look_speed = 100.0f;

        if (glfwJoystickPresent(GLFW_JOYSTICK_1))
        {
            int count;
            const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
            auto controller_name = glfwGetJoystickName(GLFW_JOYSTICK_1);

            glm::vec2 left_stick;
            glm::vec2 right_stick;

            if (std::strcmp(controller_name, "Wireless Controller") == 0)
            {
                left_stick = glm::vec2(axes[0], axes[1]);
                right_stick = glm::vec2(axes[2], axes[5]);
            }
            else
            {
                left_stick = glm::vec2(axes[0], axes[1]);
                right_stick = glm::vec2(axes[2], axes[3]);
            }

            if (glm::length(left_stick) > 0.1)
            {
                camera->set_position(camera->get_position() + camera->get_right() * left_stick.x * move_speed * delta_time);
                camera->UpdateDirectionVectors();
                camera->set_position(camera->get_position() - camera->get_front() * left_stick.y * move_speed * delta_time);
                camera->UpdateDirectionVectors();
            }
            
            if (glm::length(right_stick) > 0.1)
            {
                camera->set_yaw(camera->get_yaw() + right_stick.x * look_speed * delta_time);
                camera->UpdateDirectionVectors();
                camera->set_pitch(camera->get_pitch() - right_stick.y * look_speed * delta_time);
                camera->UpdateDirectionVectors();
                
            }
            

            if (camera->get_pitch() > 89.0f)
            {
                camera->set_pitch(89.0f);
                camera->UpdateDirectionVectors();
            }

            if (camera->get_pitch() < -89.0f)
            {
                camera->set_pitch(-89.0f);
                camera->UpdateDirectionVectors();
            }
        }
    }

    // Zwraca unsigned int 32, ktory ma takie samo ustawienie bitow jak przekazany float 32.
    // Przydatne do szybkich aproksymacji.
    inline unsigned int AsInt(float float_value)
    {
        return *(unsigned int*)&float_value;
    }

    // Zwraca float 32, ktory ma takie samo ustawienie bitow jak przekazany unsigned int.
    // Przydatne do szybkich aproksymacji.
    inline float AsFlaot(unsigned int uint_value)
    {
        return *(float*)&uint_value;
    }

    float NegateFloat(float f)
    {
        return AsFlaot(AsInt(f) ^ 0x80000000);
    }

} //utility

#endif // !UTILITY_H

