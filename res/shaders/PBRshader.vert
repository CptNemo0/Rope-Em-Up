#version 330 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec3 iv_normal;
layout (location = 2) in vec2 iv_texture;

out vec3 if_fragment_position;
out vec3 if_normal;
out vec2 if_texture;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;