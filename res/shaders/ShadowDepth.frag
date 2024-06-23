#version 330 core
float near = 1;
float far = 2000.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    gl_FragDepth = LinearizeDepth(gl_FragCoord.z) / far;
}