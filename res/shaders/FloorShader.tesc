#version 460 core
layout (vertices=4) out;

in vec2 uv_tc[];

out vec2 uv_te[];

uniform int TessLevelOuter0 = 2;
uniform int TessLevelOuter1 = 2;
uniform int TessLevelOuter2 = 2;
uniform int TessLevelOuter3 = 2;
uniform int TessLevelInner0 = 2;
uniform int TessLevelInner1 = 2;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    uv_te[gl_InvocationID] = uv_tc[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 128 * 4;
        gl_TessLevelOuter[1] = 128 * 4;
        gl_TessLevelOuter[2] = 128 * 4;
        gl_TessLevelOuter[3] = 128 * 4;

        gl_TessLevelInner[0] = 128 * 2;
        gl_TessLevelInner[1] = 128 * 2;

        gl_TessLevelOuter[0] = TessLevelOuter0;
        gl_TessLevelOuter[1] = TessLevelOuter1;
        gl_TessLevelOuter[2] = TessLevelOuter2;
        gl_TessLevelOuter[3] = TessLevelOuter3;
        gl_TessLevelInner[0] = TessLevelInner0;
        gl_TessLevelInner[1] = TessLevelInner1;
    }
}