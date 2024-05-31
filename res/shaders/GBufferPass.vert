#version 430 core
layout (location = 0) in vec3 iv_position;
layout (location = 1) in vec3 iv_normal;
layout (location = 2) in vec2 iv_texture;
layout (location = 3) in vec3 iv_tangent;
layout (location = 4) in vec3 iv_bitangent;
layout (location = 5) in ivec4 iv_boneIds;
layout (location = 6) in vec4 iv_weights;

out vec3 world_position;
out vec3 view_position;
out vec2 uv;
out mat3 normal_world_matrix;
out mat3 normal_view_matrix;
out vec3 T;
out vec3 B;
out vec3 N;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform bool useBones;
uniform int numBones;

const int MAX_BONE_INFLUENCE = 4;

layout(std430, binding = 0) buffer Bones {
    mat4 finalBonesMatrices[];
};


void main()
{
    if(useBones)
    {
		vec4 totalPosition = vec4(0.0);
		vec3 totalNormal = vec3(0.0);
		vec3 totalTangent = vec3(0.0);
		vec3 totalBitangent = vec3(0.0);
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++) 
		{
			if (iv_boneIds[i] == -1) 
				continue;
			if (iv_boneIds[i] >= numBones) 
			{
				totalPosition = vec4(iv_position, 1.0);
				totalNormal = iv_normal;
				totalTangent = iv_tangent;
				totalBitangent = iv_bitangent;
				break;
			}
			vec4 localPosition = finalBonesMatrices[iv_boneIds[i]] * vec4(iv_position, 1.0);
			totalPosition += localPosition * iv_weights[i];
			totalNormal += (mat3(finalBonesMatrices[iv_boneIds[i]]) * iv_normal) * iv_weights[i];
			totalTangent += (mat3(finalBonesMatrices[iv_boneIds[i]]) * iv_tangent) * iv_weights[i];
			totalBitangent += (mat3(finalBonesMatrices[iv_boneIds[i]]) * iv_bitangent) * iv_weights[i];
		}
		vec4 internal_world_position = model_matrix * totalPosition;
		vec4 internal_view_position = view_matrix * internal_world_position;

		world_position = internal_world_position.xyz;
		view_position = internal_view_position.xyz;

		uv = iv_texture;

		normal_world_matrix = transpose(inverse(mat3(model_matrix)));
		normal_view_matrix = transpose(inverse(mat3(view_matrix * model_matrix)));

		gl_Position = projection_matrix * internal_view_position;

		T = normalize(vec3(model_matrix * vec4(totalTangent,   0.0)));
		B = normalize(vec3(model_matrix * vec4(totalBitangent, 0.0)));
		N = normalize(vec3(model_matrix * vec4(totalNormal,    0.0)));
		return;
    
    }
	else
	{

		vec4 internal_world_position = model_matrix * vec4(iv_position, 1.0);
		vec4 internal_view_position = view_matrix * internal_world_position;

		world_position = internal_world_position.xyz;
		view_position = internal_view_position.xyz;

		uv = iv_texture;

		normal_world_matrix = transpose(inverse(mat3(model_matrix)));
		normal_view_matrix = transpose(inverse(mat3(view_matrix * model_matrix)));
	
		gl_Position = projection_matrix * internal_view_position;

		T = normalize(vec3(model_matrix * vec4(iv_tangent,   0.0)));
		B = normalize(vec3(model_matrix * vec4(iv_bitangent, 0.0)));
		N = normalize(vec3(model_matrix * vec4(iv_normal,    0.0)));
	}
}