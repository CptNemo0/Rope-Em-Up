#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;

out vec2 og_texcoord;
out vec4 og_color;

const vec4 right = vec4(-1.0, 0.0, 0.0, 0.0);
const vec4 up = vec4(0.0, 1.0, 0.0, 0.0);

vec4 view_pos = gl_in[0].gl_Position;
vec4 rightSize = right * view_pos.z;
vec4 upSize = up * view_pos.z;
vec4 sumRightUpSize = rightSize + upSize;
vec4 diffRightUpSize = rightSize - upSize;

void main()
{
    gl_Position = projection_matrix * (view_pos - model_matrix * sumRightUpSize);
    og_texcoord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = projection_matrix * (view_pos - model_matrix * diffRightUpSize);
    og_texcoord = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = projection_matrix * (view_pos + model_matrix * diffRightUpSize);
    og_texcoord = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = projection_matrix * (view_pos + model_matrix * sumRightUpSize);
    og_texcoord = vec2(1.0, 1.0);
    EmitVertex();
}