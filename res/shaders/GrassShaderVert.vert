#version 430
layout (location = 0) in vec3 local_position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tx;
layout (location = 3) in vec2 world_pos;

out vec3 out_world_position;
out vec3 out_normal;
out vec3 out_view_position;
out vec2 out_uv;
out mat3 out_normal_world_matrix;
out mat3 out_normal_view_matrix;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

uniform float time;

const float PI = 3.14159265358979323846;

float hash2(vec2 sample_) 
{
    vec3 point = fract(vec3(sample_.xyx) * 0.62); 
    point += dot(point, point.yzx + 1.2345); 
    return fract((point.x + point.y) * point.z) * 2.0 - 1.0;
}

float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float noise(vec2 p, float freq ){
	float unit = 1920.0/freq;
	vec2 ij = floor(p/unit);
	vec2 xy = mod(p,unit)/unit;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(PI*xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

float pNoise(vec2 p, int res){
	float persistance = .5;
	float n = 0.;
	float normK = 0.;
	float f = 4.;
	float amp = 1.;
	int iCount = 0;
	for (int i = 0; i<50; i++){
		n+=amp*noise(p, f);
		f*=2.;
		normK+=amp;
		amp*=persistance;
		if (iCount == res) break;
		iCount++;
	}
	float nf = n/normK;
	return nf*nf*nf*nf;
}

void main()
{
    noise2(world_pos.xy);

    mat4 translation_matrix = mat4(1.0);
    translation_matrix[3][0] = world_pos.x;
    translation_matrix[3][1] = 0.0;
    translation_matrix[3][2] = world_pos.y;
    translation_matrix[3][3] = 1.0;

    float hash = hash2(world_pos.xy);

    float cos_angle = cos(hash);
    float sin_angle = sin(hash);

    mat4 rotation_matrix_y = mat4(
        vec4(cos_angle, 0.0, -sin_angle, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(sin_angle, 0.0, cos_angle, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    float scale = 0.8 + (1.2 - 0.8) * hash;

    mat4 scale_matrix = mat4(1.0);
    scale_matrix[0][0] = 1.0;
    scale_matrix[1][1] = 2.0;
    scale_matrix[2][2] = 1.0;


    // ruch
    float wind = pNoise(world_pos + time * 25 + 1000, 100) * 5;
   
    //zakrzywienie 

    float max_lean = 1.0472 / 5;
    float height = local_position.y * local_position.y;
    float lean = max_lean * height;
    float rescaled_lean = lean * hash + wind;

    if(rescaled_lean > 1.57079632679)
    {
        rescaled_lean = 1.57079632679;
    }

    cos_angle = cos(rescaled_lean);
    sin_angle = sin(rescaled_lean);

    mat4 rotation_matrix_x = mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(1.0, cos_angle, -sin_angle, 0.0),
        vec4(1.0, sin_angle,  cos_angle, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    mat4 model_matrix = translation_matrix * rotation_matrix_y * scale_matrix * rotation_matrix_x;
    
    vec4 world_position = (model_matrix * vec4(local_position, 1.0));
    vec4 view_position = (view_matrix * world_position);

    mat3 normal_world_matrix = transpose(inverse(mat3(model_matrix)));
	mat3 normal_view_matrix = transpose(inverse(mat3(view_matrix * model_matrix)));

    gl_Position = projection_matrix * view_position;

    out_world_position = world_position.xyz;
    out_normal = normal;
    out_view_position = view_position.xyz;
    out_uv = tx;
    out_normal_world_matrix = normal_world_matrix;
    out_normal_view_matrix = normal_view_matrix;
}