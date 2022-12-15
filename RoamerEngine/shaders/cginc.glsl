//? #version 460

#define NUM_LIGHTS 256
#define NUM_DIRECT_SHADOWMAP 8
#define NUM_POINT_SHADOWMAP 8

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

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

layout(std140, binding = 0) uniform Camera {
	vec3 viewPos;
	float _Time;
	mat4 view;
	mat4 proj;
};

layout(std140, binding = 2) uniform Lights {
	int numLights;
	int numDirectShadows;
	int numPointShadows;
	float farPlane;
	vec4 globalAmbient;
	mat4 lightSpaceMatrices[NUM_DIRECT_SHADOWMAP];
	Light lights[NUM_LIGHTS];
};

uniform mat4 model;

uniform sampler2D _MainTex;
uniform sampler2D _SpecTex;
uniform vec4 _Color;

uniform Material material;

uniform sampler2D depthMaps[NUM_DIRECT_SHADOWMAP];
uniform samplerCube depthCubemaps[NUM_POINT_SHADOWMAP];