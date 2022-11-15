#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vTangent;
layout (location = 3) in vec4 vColor;
layout (location = 4) in vec2 vUV;

out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingHalfVector;

struct PositionalLight
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 _Color;

void main()
{
	varyingVertPos = (view*model*vec4(vPosition, 1.0)).xyz;
	varyingLightDir = (view*model*vec4(light.position, 0.0)).xyz - varyingVertPos;
	varyingNormal = (transpose(inverse(view * model)) * vec4(vNormal, 1.0)).xyz;
	varyingHalfVector = normalize(normalize(varyingLightDir) + normalize(-varyingVertPos)).xyz;

    gl_Position = proj * view * model * vec4(vPosition, 1.0);
    TexCoord = vUV;
}
