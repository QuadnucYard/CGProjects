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
	vec4 FragPosLightSpace[8];
	mat3 TBN;
} v2f;

uniform mat4 model;

layout(std140, binding = 0) uniform Camera {
	vec3 viewPos;
	float _Time;
	mat4 view;
	mat4 proj;
};

struct Light {
	int type;
	float range;
	float cutOff;
	float outerCutOff;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
	int shadows;
	vec3 direction;
	float shadowStrength;
};

layout(std140, binding = 2) uniform Lights {
	int numLights;
	int numDirectShadows;
	int numPointShadows;
	float farPlane;
	vec4 globalAmbient;
	mat4 lightSpaceMatrices[8];
	Light lights[256];
};

uniform sampler2D _DisplaceTex;

void main() {	
	//Displace texture
	float delta = texture(_DisplaceTex, aTexCoords).r;
	vec3 newPos = aPosition + aNormal * (delta-1)*0.05;

	v2f.FragPos = vec3(model * vec4(newPos, 1.0));
	v2f.Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
	v2f.TexCoords = aTexCoords;
	for (int i = 0; i < numDirectShadows; i++)
		v2f.FragPosLightSpace[i] = lightSpaceMatrices[i] * vec4(v2f.FragPos, 1.0);

	gl_Position = proj * view * vec4(v2f.FragPos, 1.0);

	//space of tangents
	vec3 T = normalize(vec3(model * aTangent));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	//Gram-Schmidt re-orthogonalize
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);

	v2f.TBN = mat3(T, B, N);
}
