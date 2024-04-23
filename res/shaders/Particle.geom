#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in OV
{
    float size;
    vec4 color;
} ig[];

uniform mat4 projection_matrix;

out vec2 og_texcoord;
out vec4 og_color;

const vec4 right = vec4(-1.0, 0.0, 0.0, 0.0);
const vec4 up = vec4(0.0, 1.0, 0.0, 0.0);

void main()
{
    gl_Position = projection_matrix * (gl_in[0].gl_Position + right * ig[0].size + up * ig[0].size);
    og_texcoord = vec2(0.0, 0.0);
    og_color = ig[0].color;
    EmitVertex();

    gl_Position = projection_matrix * (gl_in[0].gl_Position + right * ig[0].size - up * ig[0].size);
    og_texcoord = vec2(0.0, 1.0);
    og_color = ig[0].color;
    EmitVertex();

    gl_Position = projection_matrix * (gl_in[0].gl_Position - right * ig[0].size + up * ig[0].size);
    og_texcoord = vec2(1.0, 0.0);
    og_color = ig[0].color;
    EmitVertex();

    gl_Position = projection_matrix * (gl_in[0].gl_Position - right * ig[0].size - up * ig[0].size);
    og_texcoord = vec2(1.0, 1.0);
    og_color = ig[0].color;
    EmitVertex();
}