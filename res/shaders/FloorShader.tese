// tessellation evaluation shader
#version 460 core

layout (quads, fractional_odd_spacing, cw) in;

in vec2 uv_te[];

out vec3 world_position;
out vec2 uv;

uniform vec2 position;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform sampler2D height_map;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = uv_te[0];
    vec2 t01 = uv_te[1];
    vec2 t10 = uv_te[2];
    vec2 t11 = uv_te[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 tex_coord = (t1 - t0) * v + t0;

    mat4 translation_matrix = mat4(1.0);
    translation_matrix[3][0] = position.x;
    translation_matrix[3][1] = 0.0;
    translation_matrix[3][2] = position.y;
    translation_matrix[3][3] = 1.0;

    mat4 scale_matrix = mat4(1.0);
    scale_matrix[0][0] = 8.0;
    scale_matrix[1][1] = 1.0;
    scale_matrix[2][2] = 8.0;
    scale_matrix[3][3] = 1.0;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    vec4 scaled_p = (p + 1) * 0.5;
    tex_coord.x = scaled_p.x;
    tex_coord.y = scaled_p.z;

    float height = texture(height_map, tex_coord).y;
    height = -10.0 * height * height * height * height;
    p += vec4(0.0, height, 0.0, 0.0);

    vec4 internal_world_pos = translation_matrix * scale_matrix * p;

    gl_Position = projection_matrix * view_matrix * internal_world_pos;


    world_position = internal_world_pos.xyz;
    uv = tex_coord;
}