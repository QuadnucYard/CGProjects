#version 460
#include "cginc.glsl"

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec2 aTexCoords;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace[NUM_DIRECT_SHADOWMAP];
} v2f;

void main() {	
	v2f.FragPos = vec3(model * vec4(aPosition, 1.0));
	v2f.Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
	v2f.TexCoords = aTexCoords;
	for (int i = 0; i < numDirectShadows; i++)
		v2f.FragPosLightSpace[i] = lightSpaceMatrices[i] * vec4(v2f.FragPos, 1.0);
	
	gl_Position = proj * view * vec4(v2f.FragPos, 1.0);
}
