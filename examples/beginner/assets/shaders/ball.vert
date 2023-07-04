#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vTangent;
layout (location = 3) in vec4 vColor;
layout (location = 4) in vec2 vUV;

out vec4 outColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 _Color;
uniform float _Time;

void main()
{
    vec3 pos = vPosition;
    float k = sin(_Time * 10) * 0.2 + 1;
    float m = sin(pos.x * pos.x + pos.y * pos.z) * 4;
    pos = vec3(pos.x * k * m, pos.y * k * m, pos.z * k * m);
    gl_Position = proj * view * model * vec4(pos, 1.0);
    outColor = vColor * _Color;
    TexCoord = vUV;
}
