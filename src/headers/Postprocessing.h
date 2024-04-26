#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "glad/glad.h"
#include <stdlib.h>   
#include "Shader.h"

const float kQuadVertices[] =
{
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

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
        unsigned int rbo_;

        std::shared_ptr<Shader> shader_;

        Postprocessor(int width, int height, std::shared_ptr<Shader> shader);

        void Init();
        void Destroy();
        void Bind();
        void Draw();
    };
}





#endif // !POSTPROCESSING_H