#ifndef UTILITY_H
#define UTILITY_H

#include <fstream>  
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

namespace collisions
{
    struct Simplex
    {
        glm::vec3 points[4];
        int size;

        Simplex()
        {
            points[0] = glm::vec3(0.0f);
            points[1] = glm::vec3(0.0f);
            points[2] = glm::vec3(0.0f);
            points[3] = glm::vec3(0.0f);
            this->size = 0;
        }

        void PushFront(glm::vec3 point)
        {
            points[3] = points[2];
            points[2] = points[1];
            points[1] = points[0];
            points[0] = point;

            size = std::min(size + 1, 4);
        }
    };

    bool SameDir(glm::vec3 a, glm::vec3 b)
    {
        return (glm::dot(a, b) > 0);
    }

    glm::vec3 Perpendicular(glm::vec3 a, glm::vec3 b)
    {
        return glm::cross(glm::cross(a, b), a);
    }

    glm::vec3 Support(const std::shared_ptr <collisions::ConvexHull> A, const std::shared_ptr <collisions::ConvexHull> B, glm::vec3 direction)
    {
        int idx1 = A->FindFurthestPoint(direction);
        int idx2 = B->FindFurthestPoint(-direction);

        return A->vertices_[idx1] - B->vertices_[idx2];
    }

    std::vector<glm::vec3> MinkowskisDifference(const std::shared_ptr < collisions::ConvexHull> A, const std::shared_ptr <collisions::ConvexHull> B)
    {
        auto minkowski = std::vector<glm::vec3>();
        auto start_dir_vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        float current_rotation = 0;
        int precision = fmax(A->vertices_.size(), B->vertices_.size());
        int angle = 360 / precision;

        for (int i = 0; i < precision; i++)
        {
            current_rotation = angle * i;
            auto rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(current_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            auto rotated_vec4 = rotation_matrix * start_dir_vec;
            auto direction = glm::vec3(rotated_vec4.x, rotated_vec4.y, rotated_vec4.z);

            minkowski.push_back(Support(A, B, direction));
        }
        return minkowski;
    }

    bool Line(Simplex& simplex, glm::vec3& direction)
    {
        auto a = simplex.points[0];
        auto b = simplex.points[1];

        auto ab = b - a;
        auto ao = - a;

        if (SameDir(ab, ao))
        {
            direction = Perpendicular(ab, ao);
        }
        else
        {
            simplex.points[0] = a;
            simplex.points[1] = glm::vec3(0.0f);
            simplex.points[2] = glm::vec3(0.0f);
            simplex.points[3] = glm::vec3(0.0f);
            simplex.size = 1;

            direction = ao;
        }

        return false;
    }

    bool Triangle(Simplex& simplex, glm::vec3& direction)
    {
        glm::vec3 a = simplex.points[0];
        glm::vec3 b = simplex.points[1];
        glm::vec3 c = simplex.points[2];

        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 ao = -a;
        
        glm::vec3 abc = glm::cross(ab, ac);

        if (SameDir(glm::cross(abc, ac), ao))
        {
            if (SameDir(ac, ao))
            {
                simplex.points[0] = a;
                simplex.points[1] = c;
                simplex.points[2] = glm::vec3(0.0f);
                simplex.points[3] = glm::vec3(0.0f);
                simplex.size = 2;
                direction = Perpendicular(ac, ao);
            }
            else
            {
                simplex.points[0] = a;
                simplex.points[1] = b;
                simplex.points[2] = glm::vec3(0.0f);
                simplex.points[3] = glm::vec3(0.0f);
                simplex.size = 2;
                return Line(simplex, direction);
            }
        }
        else
        {
            if (SameDir(glm::cross(ab, abc), ao))
            {
                simplex.points[0] = a;
                simplex.points[1] = b;
                simplex.points[2] = glm::vec3(0.0f);
                simplex.points[3] = glm::vec3(0.0f);
                simplex.size = 2;
                return Line(simplex, direction);
            }
            else
            {
                if (SameDir(abc, ao))
                {
                    direction = abc;
                }
                else
                {
                    simplex.points[0] = a;
                    simplex.points[1] = c;
                    simplex.points[2] = b;
                    simplex.points[3] = glm::vec3(0.0f);
                    simplex.size = 3;
                    direction = -abc;
                }
            }
        }

        return false;
    }

    bool Tetrahedron(Simplex& simplex, glm::vec3& direction)
    {
        glm::vec3 a = simplex.points[0];
        glm::vec3 b = simplex.points[1];
        glm::vec3 c = simplex.points[2];
        glm::vec3 d = simplex.points[3];

        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 ad = d - a;
        glm::vec3 ao = -a;

        glm::vec3 abc = glm::cross(ab, ac);
        glm::vec3 acd = glm::cross(ac, ad);
        glm::vec3 adb = glm::cross(ad, ab);

        if (SameDir(abc, ao))
        {
            simplex.points[0] = a;
            simplex.points[1] = b;
            simplex.points[2] = c;
            simplex.points[3] = glm::vec3(0.0f);
            simplex.size = 3;
            return Triangle(simplex, direction);
        }

        if (SameDir(acd, ao))
        {
            simplex.points[0] = a;
            simplex.points[1] = c;
            simplex.points[2] = d;
            simplex.points[3] = glm::vec3(0.0f);
            simplex.size = 3;
            return Triangle(simplex, direction);
        }
        
        if (SameDir(adb, ao))
        {
            simplex.points[0] = a;
            simplex.points[1] = d;
            simplex.points[2] = b;
            simplex.points[3] = glm::vec3(0.0f);
            simplex.size = 3;
            return Triangle(simplex, direction);
        }
        return true;
    }

    bool NearestSimplex(Simplex& simplex, glm::vec3& direction)
    {
        switch (simplex.size)
        {
        case 2:
            return Line(simplex, direction);
            break;
        case 3:
            return Triangle(simplex, direction);
            break;
        case 4: 
            return Tetrahedron(simplex, direction);
            break;
        }
        return false;
    }

    bool GJK(const std::shared_ptr<collisions::ConvexHull> A, const std::shared_ptr<collisions::ConvexHull> B)
    {
        Simplex simplex = Simplex();
            glm::vec3 initial_support = Support(A, B, glm::vec3(1.0f, 0.0f, 0.0f));
        
        simplex.PushFront(initial_support);

        glm::vec3 new_direction = -initial_support;

        while (1)
        {
            initial_support = Support(A, B, new_direction);

            if (glm::dot(initial_support, new_direction) <= 0)
            {
                return false;
            }

            simplex.PushFront(initial_support);
            
            if (NearestSimplex(simplex, new_direction))
            {
                return true;
            }
        }
    }

    bool InsideDifference(const std::vector<glm::vec3>& polygon) 
    {
        static glm::vec3 normal(0.0f, 1.0f, 0.0f);
        int size = polygon.size();
        assert(size >= 3);

        for (int i = 0; i < size; i++)
        {
            glm::vec3 edge = polygon[(i + 1) % size] - polygon[i];
            /*float y = edge.z * polygon[i].x - edge.x * polygon[i].z;

            if (y >= 0)
            {
                return false;
            }*/

            auto cross = glm::cross(edge, polygon[i]); // Jezeli jest kolizja to wszystkie corss producty beda szly w kierunku (0, 1, 0)
            auto dot = glm::dot(cross, normal);

            if (dot > 0)
            {
                return false; 
            }
        }

        return true;
    }

    void WriteDebugFIles(const std::vector<glm::vec3>& polygon, const std::shared_ptr<collisions::ConvexHull> A, const std::shared_ptr<collisions::ConvexHull> B)
    {
        std::ofstream polygon_file("res/logs/polygon_debug.csv");
        for (int i = 0; i < polygon.size(); i++)
        {
            polygon_file << polygon[i].x << ", " << polygon[i].z << "\n";
        }
        polygon_file.close();

        std::ofstream coll_a_file("res/logs/coll_a_debug.csv");
        for (int i = 0; i < A->vertices_.size(); i++)
        {
            coll_a_file << A->vertices_[i].x << ", " << A->vertices_[i].z << "\n";
        }
        coll_a_file.close();

        std::ofstream coll_b_file("res/logs/coll_b_debug.csv");
        for (int i = 0; i < B->vertices_.size(); i++)
        {
            coll_b_file << B->vertices_[i].x << ", " << B->vertices_[i].z << "\n";
        }
        coll_b_file.close();
    }
}

#endif // !UTILITY_H

