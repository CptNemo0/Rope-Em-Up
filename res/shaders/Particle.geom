#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in OV
{
    float size;
    vec4 color;
    float rotation;
} ig[];

uniform mat4 projection_matrix;

out vec2 og_texcoord;
out vec4 og_color;

const vec4 right = vec4(-1.0, 0.0, 0.0, 0.0);
const vec4 up = vec4(0.0, 1.0, 0.0, 0.0);

vec4 pos = gl_in[0].gl_Position;
vec4 color = ig[0].color;
float size = ig[0].size;
vec4 rightSize = right * size;
vec4 upSize = up * size;
vec4 sumRightUpSize = rightSize + upSize;
vec4 diffRightUpSize = rightSize - upSize;

mat4 createRotationMatrix(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return mat4(
        vec4(c, -s, 0.0, 0.0),
        vec4(s,  c, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

void main()
{
    mat4 rotationMatrix = createRotationMatrix(ig[0].rotation);

    gl_Position = projection_matrix * (pos + rotationMatrix * sumRightUpSize);
    og_texcoord = vec2(0.0, 0.0);
    og_color = color;
    EmitVertex();

    gl_Position = projection_matrix * (pos + rotationMatrix * diffRightUpSize);
    og_texcoord = vec2(0.0, 1.0);
    og_color = color;
    EmitVertex();

    gl_Position = projection_matrix * (pos - rotationMatrix * diffRightUpSize);
    og_texcoord = vec2(1.0, 0.0);
    og_color = color;
    EmitVertex();

    gl_Position = projection_matrix * (pos - rotationMatrix * sumRightUpSize);
    og_texcoord = vec2(1.0, 1.0);
    og_color = color;
    EmitVertex();
}