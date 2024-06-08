#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "typedef.h"
#include "Lights.h"

class Shader
{
private:


public:
    unsigned int id_; // bylo wczesniej prywatne ale testuje // xD i nie zmieniles na private potem
    Shader() = default;
    Shader(const string& vertex_shader_path, const string& fragment_shader_path);
    Shader(const string& vertex_shader_path, const string& geometry_shader_path, const string& fragment_shader_path);

    string v_path, f_path, g_path;
    
    ~Shader();

    void Use();
    void End();

    unsigned int get_id() const;

    void SetBool(const string& name, bool value) const;
    void SetInt(const string& name, int value) const;
    void SetFloat(const string& name, float value) const;
    void SetVec3(const string& name, glm::vec3 value) const;
    void SetVec2(const string& name, glm::vec2 value) const;
    void SetMatrix4(const string& name, glm::mat4 value) const;

    void SetLight(const string& name, Light value);
    void SetDirectionalLight(const string& name, DirectionalLight value);
    void SetPointLight(const string& name, PointLight value);
    void SetSpotLight(const string& name, SpotLight value);
};

#endif SHADER_H
