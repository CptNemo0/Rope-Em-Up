#version 330 core
layout (location = 0) out vec3 color_texture;

in vec2 if_uv;

uniform sampler2D position_texture;
uniform sampler2D normal_texture;
uniform sampler2D noise_texture;
uniform sampler2D mask_texture;

uniform int quality;

const int MAX_KERNEL_SIZE = 128;
uniform vec3 kernel[MAX_KERNEL_SIZE];

uniform float radius;
uniform float bias;

uniform int height;
uniform int width;

uniform mat4 projection_matrix;

void main()
{
	float inv_quality = 1.0 / quality;
	vec2 noise_scale = vec2(width * 0.25, height * 0.25);

	vec3 position = texture(position_texture, if_uv).xyz;
    vec3 noise = normalize(texture(noise_texture, if_uv * noise_scale).xyz);
	vec3 normal =  normalize(texture(normal_texture, if_uv).rgb * 2.0 - 1.0);

    float mask = texture(mask_texture, if_uv).r;

    if(mask > 0.01)
    {
	    vec3 tangent = normalize(noise - normal * dot(noise, normal));
        vec3 bitangent = cross(normal, tangent);
        mat3 TBN = mat3(tangent, bitangent, normal);

        float occlusion = 0.0;
        for(int i = 0; i < quality; i++)
	    {        
            vec3 sample_pos = position + (TBN * kernel[i]) * radius; 

            vec4 offset = vec4(sample_pos, 1.0);
            offset = projection_matrix * offset; 
            offset.xyz /= offset.w; 
            offset.xyz = offset.xyz * 0.5 + 0.5; 

            float sample_depth = texture(position_texture, offset.xy).z; 
        
            float range_check = smoothstep(0.0, 1.0, radius / abs(position.z - sample_depth));
            occlusion += (sample_depth > sample_pos.z + bias ? 1.0 : 0.0) * range_check;   
        }

        occlusion = occlusion * inv_quality;
        color_texture = vec3(1 - occlusion).rgb;
    }
	else
    {
        color_texture = vec3(1.0).rgb;
    }
}