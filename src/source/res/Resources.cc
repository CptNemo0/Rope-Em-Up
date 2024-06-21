#include "../../headers/res/Resources.h"

namespace res
{

std::map<string, s_ptr<Model>> models;
std::map<std::vector<string>, s_ptr<Shader>> shaders;
std::map<string, s_ptr<tmp::Texture>> textures;
std::map<string, s_ptr<Font>> fonts;
std::map<std::tuple<string, string, int>, s_ptr<anim::Animation>> animations;
std::map<string, s_ptr<audio::AudioBuffer>> sounds;

FT_Library ft;

s_ptr<Model> get_model(string path)
{
    if (models.contains(path))
    {
        return models[path];
    }
    s_ptr<Model> model = make_shared<Model>(path);
    models[path] = model;
    return model;
}

s_ptr<Shader> get_shader(string v_path, string f_path)
{
    if (shaders.contains({v_path, f_path}))
    {
        return shaders[{v_path, f_path}];
    }
    s_ptr<Shader> shader = make_shared<Shader>(v_path, f_path);
    shaders[{v_path, f_path}] = shader;
    return shader;
}

s_ptr<Shader> get_shader(string v_path, string g_path, string f_path)
{
    if (shaders.contains({v_path, g_path, f_path}))
    {
        return shaders[{v_path, g_path, f_path}];
    }
    s_ptr<Shader> shader = make_shared<Shader>(v_path, g_path, f_path);
    shaders[{v_path, g_path, f_path}] = shader;
    return shader;
}

s_ptr<Shader> get_shader(string v_path, string tcs_path, string tes_path, string f_path)
{
    if (shaders.contains({ v_path, tcs_path, tes_path, f_path }))
    {
        return shaders[{v_path, tcs_path, tes_path, f_path}];
    }
    s_ptr<Shader> shader = make_shared<Shader>(v_path, tcs_path, tes_path, f_path);
    shaders[{v_path, tcs_path, tes_path, f_path}] = shader;
    return shader;
}

s_ptr<tmp::Texture> get_texture(string path)
{
    if (textures.contains(path))
    {
        return textures[path];
    }
    s_ptr<tmp::Texture> texture = make_shared<tmp::Texture>(path);
    textures[path] = texture;
    return texture;
}

s_ptr<Font> get_font(string path)
{
    if (fonts.contains(path))
    {
        return fonts[path];
    }
    s_ptr<Font> font = make_shared<Font>(ft, path.c_str());
    fonts[path] = font;
    return font;
}

s_ptr<anim::Animation> get_animation(string anim_path, int anim_number, string model_path)
{
    if (animations.contains({anim_path, model_path, anim_number}))
    {
        return animations[{anim_path, model_path, anim_number}];
    }
    s_ptr<anim::Animation> anim = make_shared<anim::Animation>(anim_path, anim_number, get_model(model_path));
    animations[{anim_path, model_path, anim_number}] = anim;
    return anim;
}

s_ptr<audio::AudioBuffer> get_sound(string path)
{
    if (sounds.contains(path))
    {
        return sounds[path];
    }
    s_ptr<audio::AudioBuffer> sound = audio::AudioManager::i_->LoadSound(path);
    sounds[path] = sound;
    return sound;
}

std::map<string, s_ptr<audio::AudioBuffer>> &get_all_sounds()
{
    return sounds;
}

int init_freetype()
{
    if (FT_Init_FreeType(&ft))
    {
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
        return -1;
    }
    return 0;
}

} // namespace res
