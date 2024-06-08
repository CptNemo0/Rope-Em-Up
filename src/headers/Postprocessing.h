#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "glad/glad.h"
#include <stdlib.h>

#include "typedef.h"
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

        float gamma_;
        float brightness_;
        float contrast_;
        float vignete_contrast_;
        float vignete_amount_;
        float noise_amount_;
        float transition_vignette_time_;
        float transition_vignette_current_time_;
        bool slowed_time;
        
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