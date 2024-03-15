#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace utility
{
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

        window = glfwCreateWindow(mode->width, mode->height, window_name.c_str(), monitor, nullptr);

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

