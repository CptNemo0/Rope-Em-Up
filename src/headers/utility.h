#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <stdlib.h>
#include <unordered_map>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

#include "Camera.h"

namespace utility
{
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

    int InitGLFW(GLFWwindow*& window, GLFWmonitor*& monitor, GLFWvidmode*& mode, const string& window_name)
    {
        int return_value = 0;
        if (!glfwInit())
        {
            return_value = -1;
        }

        monitor = glfwGetPrimaryMonitor();
        mode = (GLFWvidmode*)glfwGetVideoMode(monitor);

        // SET CUSTOM RESOLUTION
        // mode->width = 800;
        // mode->height = 800;

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
        // VSYNC
        // glfwSwapInterval(1);
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
        //glDebugMessageCallback(MessageCallback, 0);
        return return_value;
    }

    void InitImGUI(GLFWwindow*& window)
    {
        bool return_value = false;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        return_value = ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
        return_value *= ImGui_ImplOpenGL3_Init("#version 330");
    }

    void DebugCameraMovement(GLFWwindow* window, s_ptr <llr::Camera> camera, float delta_time)
    {
        static float move_speed = 5.0f;
        static float look_speed = 200.0f;

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
    
    void DebugCameraMovementJoystick(GLFWwindow* window, s_ptr <llr::Camera> camera, float delta_time)
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

} //utility

#endif // !UTILITY_H

