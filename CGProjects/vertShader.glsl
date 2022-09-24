#version 430

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;

out vec4 outColor;

void main()
{
    gl_Position = vec4(vPosition, 1.0);
    outColor = vColor;
}

