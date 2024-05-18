#version 330 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec3 iv_normal;
layout (location = 2) in vec2 iv_texture;
layout (location = 5) in ivec4 iv_boneIds;
layout (location = 6) in vec4 iv_weights;

out vec3 world_position;
out vec3 view_position;
out vec2 uv;
out mat3 normal_world_matrix;
out mat3 normal_view_matrix;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
	vec4 totalPosition = vec4(0.0);
    vec3 totalNormal = vec3(0.0);

	 for (int i = 0; i < MAX_BONE_INFLUENCE; i++) 
       {
        if (iv_boneIds[i] == -1) 
            continue;
        if (iv_boneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(iv_position, 1.0);
            totalNormal = iv_normal;
            break;
        }
        vec4 localPosition = finalBonesMatrices[iv_boneIds[i]] * vec4(iv_position, 1.0);
        totalPosition += localPosition * iv_weights[i];
        totalNormal += (mat3(finalBonesMatrices[iv_boneIds[i]]) * iv_normal) * iv_weights[i];
    }

    vec4 internal_world_position = model_matrix * totalPosition;
	vec4 internal_view_position = view_matrix * internal_world_position;

	world_position = internal_world_position.xyz;
	view_position = internal_view_position.xyz;

	uv = iv_texture;

	normal_world_matrix = transpose(inverse(mat3(model_matrix)));
	normal_view_matrix = transpose(inverse(mat3(view_matrix * model_matrix)));
	
	gl_Position = projection_matrix * internal_view_position;
}