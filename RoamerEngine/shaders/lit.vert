#version 460

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec2 aTexCoords;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} v2f;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 norm;

void main() {	
	v2f.FragPos = vec3(model * vec4(aPosition, 1.0));
	v2f.Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
	v2f.TexCoords = aTexCoords;
	
	gl_Position = proj * view * vec4(v2f.FragPos, 1.0);
}
