#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "glad/glad.h"
#include <stdlib.h>

#include "global.h"
#include "Shader.h"

namespace ppc
{
    class Postprocessor
    {
    public:
        int width_;
        int height_;

        unsigned int vao_;
        unsigned int vbo_;
        unsigned int framebuffer_;
        unsigned int texture_color_buffer_;
        unsigned int texture_depth_buffer_;
        unsigned int rbo_;

        float gamma_;
        float brightness_;
        float contrast_;

        s_ptr<Shader> shader_;

        Postprocessor(int height, int width, s_ptr<Shader> shader);

        void Init();
        void Destroy();
        void Bind();
        void Unbind();
        void Draw();

        void Update();
    };
}





#endif // !POSTPROCESSING_H