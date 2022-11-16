#version 460

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec2 aTexCoord;

out VS_OUT {
	vec4 Color;
	vec2 TexCoord;
} v2f;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 _Color;

void main()
{
	gl_Position = proj * view * model * vec4(aPosition, 1.0);
	v2f.Color = aColor * _Color;
	v2f.TexCoord = aTexCoord;
}
