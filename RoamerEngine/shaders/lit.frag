#version 460

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} v2f;

out vec4 FragColor;

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

uniform mat4 model;
uniform sampler2D _MainTex;
uniform sampler2D _SpecTex;
uniform vec4 _Color;
uniform Material material;
uniform samplerCube depthMaps[16];

layout(std140, binding = 0) uniform Camera {
	vec3 viewPos;
	mat4 view;
	mat4 proj;
};

layout(std140, binding = 2) uniform Lights {
	int numLights;
	float farPlane;
	vec4 globalAmbient;
	Light lights[256];
};

float ShadowCalculation(Light light);
vec3 CalcDirLight(Light light, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 viewDir);

void main() {
	
	vec3 viewDir = normalize(viewPos - v2f.FragPos);
	vec3 result;
	for (int i = 0; i < numLights; i++) {
		if (lights[i].type == 0) result += CalcDirLight(lights[i], viewDir);
		if (lights[i].type == 1) result += CalcPointLight(lights[i], viewDir);
		if (lights[i].type == 2) result += CalcSpotLight(lights[i], viewDir);
	}
	
	FragColor = vec4(result, 1.0f);
	//FragColor = texture(_MainTex, v2f.TexCoords);
	//FragColor = vec4(1.0f);
}

// array of offset direction for sampling
const vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(Light light)
{
	const float bias = 0.05;
	float shadow = 0.0;
	// Get vector between fragment position and light position
	vec3 fragToLight = v2f.FragPos - light.position;
	// Now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);

	if (light.shadows > 0) { // Hard
		// Use the light to fragment vector to sample from the depth map    
		float closestDepth = texture(depthMaps[light.shadows - 1], fragToLight).r;
		// It is currently in linear range between [0,1]. Re-transform back to original value
		closestDepth *= farPlane;
		shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	} else { // Soft
		const int samples = 20;
		float viewDistance = length(viewPos - v2f.FragPos);
		float diskRadius = (1.0 + (viewDistance / farPlane)) / farPlane;
		for(int i = 0; i < samples; ++i) {
			float closestDepth = texture(depthMaps[-light.shadows - 1], fragToLight + gridSamplingDisk[i] * diskRadius).r;
			closestDepth *= farPlane;   // undo mapping [0;1]
			if(currentDepth - bias > closestDepth)
				shadow += 1.0;
		}
		shadow /= float(samples);
	}
	return 1 - shadow * light.shadowStrength;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(Light light, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(v2f.Normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, v2f.Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = ((globalAmbient + light.ambient) * texture(_MainTex, v2f.TexCoords) * material.ambient).rgb;
	vec3 diffuse = (light.diffuse * diff * texture(_MainTex, v2f.TexCoords) * material.diffuse).rgb;
	vec3 specular = (light.specular * spec * material.specular).rgb;
	return ambient + diffuse + specular;
}

// calculates the color when using a point light.
vec3 CalcPointLight(Light light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - v2f.FragPos);
	// diffuse shading
	float diff = max(dot(v2f.Normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, v2f.Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float dist = length(light.position - v2f.FragPos) / light.range;
	float attenuation = 1.0 / (1 + 4.6 * dist + 80.0 * dist * dist);
	// combine results
	vec3 ambient = ((globalAmbient + light.ambient) * texture(_MainTex, v2f.TexCoords) * material.ambient).rgb;
	vec3 diffuse = (light.diffuse * diff * texture(_MainTex, v2f.TexCoords) * material.diffuse).rgb;
	vec3 specular = (light.specular * spec * material.specular).rgb;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	float shadow = ShadowCalculation(light);
	return ambient + shadow * (diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(Light light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - v2f.FragPos);
	// diffuse shading
	float diff = max(dot(v2f.Normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, v2f.Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float dist = length(light.position - v2f.FragPos) / light.range;
	float attenuation = 1.0 / (1 + 4.6 * dist + 80.0 * dist * dist);
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction)); 
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// combine results
	vec3 ambient = ((globalAmbient + light.ambient) * texture(_MainTex, v2f.TexCoords) * material.ambient).rgb;
	vec3 diffuse = (light.diffuse * diff * texture(_MainTex, v2f.TexCoords) * material.diffuse).rgb;
	vec3 specular = (light.specular * spec * material.specular).rgb;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return ambient + diffuse + specular;
}