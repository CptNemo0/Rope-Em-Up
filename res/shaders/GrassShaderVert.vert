#version 430
layout (location = 0) in vec3 local_position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tx;
layout (location = 3) in vec2 world_pos;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bitangent;

out vec3 out_world_position;
out vec3 out_view_position;
out vec2 out_uv;
out mat3 out_normal_world_matrix;
out mat3 out_normal_view_matrix;

out vec3 T;
out vec3 B;
out vec3 N;

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

vec4 mod289(vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}


float cnoise(vec2 P)
{
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod289(Pi); 
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;

  vec4 i = permute(permute(ix) + iy);

  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  vec4 gy = abs(gx) - 0.5 ;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
  g00 *= norm.x;
  g01 *= norm.y;
  g10 *= norm.z;
  g11 *= norm.w;

  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));

  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return (2.3 * n_xy) * 0.5 + 0.5;
}

void main()
{
    mat4 translation_matrix = mat4(1.0);
    translation_matrix[3][0] = world_pos.x;
    translation_matrix[3][1] = 0.0;
    translation_matrix[3][2] = world_pos.y;
    translation_matrix[3][3] = 1.0;

    float rothash = cnoise(2 * world_pos.xy);

    float cos_angle = cos(rothash);
    float sin_angle = sin(rothash);

    mat4 rotation_matrix_y = mat4(
        vec4(cos_angle, 0.0, -sin_angle, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(sin_angle, 0.0, cos_angle, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    float pos_hash = cnoise(world_pos.xy);
    float scale_hash = ((pos_hash * 0.75) + 0.25);

    mat4 scale_matrix = mat4(1.0);
    scale_matrix[0][0] = 1.0;
    scale_matrix[1][1] = 2.0 * scale_hash;
    scale_matrix[2][2] = 1.0;    
   
    //zakrzywienie 

    float random_sign = hash2(world_pos);
    float height = local_position.y * local_position.y;
    float lean = hash2(world_pos) * PI / (height * pos_hash * 2000.0);
   
    float wind = sin(cnoise(world_pos + time * 0.2) * 2.0 - 1.0) * 2.0 * pos_hash;

    float lean_wind = lean + wind;

    cos_angle = cos(wind);
    sin_angle = sin(wind);

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

    out_view_position = view_position.xyz;
    out_uv = tx;
    out_normal_world_matrix = normal_world_matrix;
    out_normal_view_matrix = normal_view_matrix;

    T = normalize(vec3(model_matrix * vec4(tangent,   0.0)));
    B = normalize(vec3(model_matrix * vec4(bitangent, 0.0)));
	N = normalize(vec3(model_matrix * vec4(normal,    0.0)));
}