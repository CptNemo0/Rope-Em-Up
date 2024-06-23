#version 430 core

layout(triangles) in;
layout(triangle_strip) out;
layout(max_vertices = 3) out;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

in VS_OUT
{
	vec2 uv;
} gs_in[];

out vec2 uv;

void main()
{
	mat4 pv = projection_matrix * view_matrix * model_matrix;

	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;

	gl_Position = pv * vec4(p0, 1.0);
	uv = gs_in[0].uv;
	EmitVertex();

	gl_Position = pv * vec4(p1, 1.0);
	uv = gs_in[1].uv;
	EmitVertex();

	gl_Position = pv * vec4(p2, 1.0);
	uv = gs_in[2].uv;
	EmitVertex();

	EndPrimitive();
}