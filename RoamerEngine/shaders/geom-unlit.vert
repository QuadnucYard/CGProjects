#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vTangent;
layout (location = 3) in vec4 vColor;
layout (location = 4) in vec2 vUV;

out vec4 outColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 _Color;

void main()
{
    gl_Position = proj * view * model * vec4(vPosition, 1.0);
    outColor = vColor * _Color;
}
