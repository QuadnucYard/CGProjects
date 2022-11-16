﻿#version 460

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} v2f;

out vec4 FragColor;

struct Light {
	int type;
	float range;
	float spotAngle;
	float innerSpotAngle;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
	vec3 direction;
};

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform sampler2D _MainTex;
uniform sampler2D _SpecTex;
uniform vec4 _Color;
uniform Material material;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 norm_matrix;

layout(std140, binding = 0) uniform Lights {
	vec3 viewPos;
	int numLights;
	vec4 globalAmbient;
	Light lights[256];
};

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	
	vec3 viewDir = normalize(viewPos - v2f.FragPos);
	
	// == =====================================================
	// Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
	// For each phase, a calculate function is defined that calculates the corresponding color
	// per lamp. In the main() function we take all the calculated colors and sum them up for
	// this fragment's final color.
	// == =====================================================
	vec3 result;
	for (int i = 0; i < numLights; i++) {
		if (lights[i].type == 0) result += CalcDirLight(lights[i], v2f.Normal, viewDir);
		if (lights[i].type == 1) result += CalcPointLight(lights[i], v2f.Normal, v2f.FragPos, viewDir);
		if (lights[i].type == 2) result += CalcSpotLight(lights[i], v2f.Normal, v2f.FragPos, viewDir);
	}
	
	FragColor = vec4(result, 1.0f);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = ((globalAmbient + light.ambient) * texture(_MainTex, v2f.TexCoords) * material.ambient).rgb;
	vec3 diffuse = (light.diffuse * diff * texture(_MainTex, v2f.TexCoords) * material.diffuse).rgb;
	vec3 specular = (light.specular * spec * texture(_SpecTex, v2f.TexCoords) * material.specular).rgb;
	return ambient + diffuse + specular;
}

// calculates the color when using a point light.
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float dist = length(light.position - fragPos) / light.range;
	float attenuation = 1.0 / (1 + 4.6 * dist + 80.0 * dist * dist);
	// float dist = length(light.position - fragPos);
	// float attenuation = 1.0 / (1 + 0.045 * dist + 0.0075 * dist * dist);
	// combine results
	vec3 ambient = ((globalAmbient + light.ambient) * texture(_MainTex, v2f.TexCoords) * material.ambient).rgb;
	vec3 diffuse = (light.diffuse * diff * texture(_MainTex, v2f.TexCoords) * material.diffuse).rgb;
	vec3 specular = (light.specular * spec * texture(_SpecTex, v2f.TexCoords) * material.specular).rgb;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float dist = length(light.position - fragPos) / light.range;
	float attenuation = 1.0 / (1 + 4.6 * dist + 80.0 * dist * dist);
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction)); 
	float epsilon = light.innerSpotAngle - light.spotAngle;
	float intensity = clamp((theta - light.spotAngle) / epsilon, 0.0, 1.0);
	// combine results
	vec3 ambient = ((globalAmbient + light.ambient) * texture(_MainTex, v2f.TexCoords) * material.ambient).rgb;
	vec3 diffuse = (light.diffuse * diff * texture(_MainTex, v2f.TexCoords) * material.diffuse).rgb;
	vec3 specular = (light.specular * spec * texture(_SpecTex, v2f.TexCoords) * material.specular).rgb;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return ambient + diffuse + specular;
}