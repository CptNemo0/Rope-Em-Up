#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>

#include "../Model.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Font.h"

#include "../global.h"

namespace res
{
    s_ptr<Model> get_model(string path);

    s_ptr<Shader> get_shader(string v_path, string f_path);
    s_ptr<Shader> get_shader(string v_path, string g_path, string f_path);

    s_ptr<tmp::Texture> get_texture(string path);

    s_ptr<Font> get_font(string path);

    int init_freetype();
};

#endif // RESOURCES_H