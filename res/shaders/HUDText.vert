#version 330 core

layout (location = 0) in vec4 iv_vertex; // <vec2 pos, vec2 tex>
out vec2 ov_texcoords;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = model_matrix * vec4(iv_vertex.xy, 0.0, 1.0);
    ov_texcoords = iv_vertex.zw;
}  