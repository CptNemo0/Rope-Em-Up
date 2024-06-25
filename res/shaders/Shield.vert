#version 430 core
layout (location = 0) in vec3 iv_position;

uniform vec3 position;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform float timer;
void main()
{
    mat4 translation_matrix = mat4(1.0);
    translation_matrix[3][0] = position.x;
    translation_matrix[3][1] = position.y;
    translation_matrix[3][2] = position.z;
    translation_matrix[3][3] = 1.0;

    mat4 scale_matrix = mat4(1.0);
    scale_matrix[0][0] = 1.4;
    scale_matrix[1][1] = 1.4;
    scale_matrix[2][2] = 1.4;
    scale_matrix[3][3] = 1.0;

    float sin_angle = sin(timer);
    float cos_angle = sqrt(1.0 - (sin_angle * sin_angle));
  
    mat4 rotation_matrix_y = mat4(
        vec4(cos_angle, 0.0, -sin_angle, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(sin_angle, 0.0, cos_angle, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    gl_Position = projection_matrix * view_matrix * translation_matrix * rotation_matrix_y* scale_matrix * vec4(iv_position, 1.0);
}