#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "GBuffer.h"
#include "LBuffer.h"
#include "global.h"
#include "Shader.h"
#include "components/MeshRenderer.h"
#include "glad/glad.h"
#include "Postprocessing.h"

class RenderManager
{
public:
	static RenderManager* i_;
private:
	RenderManager(GBuffer* gbuffer, LBuffer* lbuffer, ppc::Postprocessor* postprocessor, s_ptr<Shader> g_buffer_shader, s_ptr<Shader> lighting_pass_shader);
	~RenderManager() = default;
public:
    static void Initialize(GBuffer* gbuffer, LBuffer* lbuffer, ppc::Postprocessor* postprocessor, s_ptr<Shader> g_buffer_shader, s_ptr<Shader> lighting_pass_shader)
    {
        if (i_ == nullptr)
        {
            i_ = new RenderManager(gbuffer, lbuffer, postprocessor, g_buffer_shader, lighting_pass_shader);
        }
    }

    static void Destroy()
    {
        if (i_ != nullptr)
        {
            delete i_;
            i_ = nullptr;
        }
    }

    s_ptr<Shader> g_buffer_shader_;
    s_ptr<Shader> lighting_pass_shader_;

    std::vector <s_ptr<components::MeshRenderer>> renderers_;

    GBuffer* gbuffer_;
    LBuffer* lbuffer_;
    ppc::Postprocessor* postprocessor_;

    void GeometryPass();
    void LightingPass(glm::vec3 camera_position);
    void PreprocessorPass();
};

#endif // !RENDER_MANAGER_H
