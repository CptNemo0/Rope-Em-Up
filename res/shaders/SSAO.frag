#version 410 core
layout (location = 0) out vec3 color_texture;

in vec2 if_uv;

uniform sampler2D position_texture;
uniform sampler2D normal_texture;
uniform sampler2D noise_texture;

uniform int quality;

const int MAX_KERNEL_SIZE = 128;
uniform vec3 kernel[MAX_KERNEL_SIZE];

uniform float radius;
uniform float bias;

uniform int height;
uniform int width;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
	float inv_quality = 1.0 / float(quality);
	vec2 noise_scale = vec2(width * inv_quality, height * inv_quality);

	vec3 position = texture(position_texture, if_uv).rgb;
	vec3 normal =  normalize(texture(normal_texture, if_uv).rgb);
	vec3 noise = normalize(texture(noise_texture, if_uv * noise_scale).xyz);

	vec3 tangent = normalize(noise - normal * dot(noise, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for(int i = 0; i < quality; i++)
	{
        // get sample position
        vec3 sample_pos = TBN * kernel[i]; // from tangent to view-space
        sample_pos = position + sample_pos * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample_pos, 1.0);
        offset = projection_matrix * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sample_depth = texture(position_texture, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float range_check = smoothstep(0.0, 1.0, radius / abs(position.z - sample_depth));
        occlusion += (sample_depth >= sample_pos.z + bias ? 1.0 : 0.0) * range_check;           
    }

    occlusion = 1.0 - occlusion * inv_quality;
    
    color_texture = vec3(occlusion);
}