#version 460

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace[8];
	mat3 TBN;
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
uniform sampler2D _NormalTex;
uniform vec4 _Color;
uniform Material material;
uniform sampler2D depthMaps[8];
uniform samplerCube depthCubemaps[8];

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
	mat4 lightSpaceMatrices[8];
	Light lights[256];
};

float CalcDirectShadow(Light light);
float CalcPointShadow(Light light);
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


float CalcDirectShadow(Light light)
{
	int index = light.shadows > 0 ? light.shadows - 1 : -light.shadows - 1;
	vec4 fragPosLightSpace = v2f.FragPosLightSpace[index];
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(depthMaps[index], projCoords.xy).r; 
	// calculate bias (based on depth map resolution and slope)
	vec3 normal = texture(_NormalTex, v2f.TexCoords).rgb;
	normal = normalize(normal*2.0- 1.0);
	normal = normalize(v2f.TBN * normal);
	vec3 lightDir = normalize(light.position - v2f.FragPos);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	// check whether current frag pos is in shadow
	float shadow = 0.0;
	if (light.shadows > 0) { // Hard
		shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	} else { // Soft
		// PCF
		vec2 texelSize = 1.0 / textureSize(depthMaps[index], 0);
		for(int x = -1; x <= 1; ++x) {
			for(int y = -1; y <= 1; ++y) {
				float pcfDepth = texture(depthMaps[index], projCoords.xy + vec2(x, y) * texelSize).r; 
				shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
			}    
		}
		shadow /= 9.0;
	}

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if(projCoords.z > 1.0)
		shadow = 0.0;
		
	return 1 - shadow * light.shadowStrength;
}

float CalcPointShadow(Light light)
{
	const float bias = 0.05;
	float shadow = 0.0;
	// Get vector between fragment position and light position
	vec3 fragToLight = v2f.FragPos - light.position;
	// Now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);

	if (light.shadows > 0) { // Hard
		// Use the light to fragment vector to sample from the depth map    
		float closestDepth = texture(depthCubemaps[light.shadows - 1], fragToLight).r;
		// It is currently in linear range between [0,1]. Re-transform back to original value
		closestDepth *= farPlane;
		shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	} else { // Soft
		const int samples = 20;
		float viewDistance = length(viewPos - v2f.FragPos);
		float diskRadius = (1.0 + (viewDistance / farPlane)) / farPlane;
		for(int i = 0; i < samples; ++i) {
			float closestDepth = texture(depthCubemaps[-light.shadows - 1], fragToLight + gridSamplingDisk[i] * diskRadius).r;
			closestDepth *= farPlane;   // undo mapping [0;1]
			if(currentDepth - bias > closestDepth)
				shadow += 1.0;
		}
		shadow /= samples;
	}
	return 1 - shadow * light.shadowStrength;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(Light light, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	//sampling and calculate normal from texture
	vec3 normal = texture(_NormalTex, v2f.TexCoords).rgb;
	normal = normalize(normal*2.0- 1.0);
	normal = normalize(v2f.TBN * normal);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = ((globalAmbient + light.ambient) * texture(_MainTex, v2f.TexCoords) * material.ambient).rgb;
	vec3 diffuse = (light.diffuse * diff * texture(_MainTex, v2f.TexCoords) * material.diffuse).rgb;
	float matSpecular = texture(_SpecTex, v2f.TexCoords).r;
	vec3 specular = (light.specular * spec * matSpecular).rgb;
	float shadow = CalcDirectShadow(light);
	return ambient + shadow * (diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(Light light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - v2f.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	//sampling and calculate normal from texture
	vec3 normal = texture(_NormalTex, v2f.TexCoords).rgb;
	normal = normalize(normal*2.0- 1.0);
	normal = normalize(v2f.TBN * normal);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	// attenuation
	float dist = length(light.position - v2f.FragPos) / light.range;
	float attenuation = 1.0 / (1 + 4.6 * dist + 80.0 * dist * dist);
	// combine results
	vec3 ambient = ((globalAmbient + light.ambient) * texture(_MainTex, v2f.TexCoords) * material.ambient).rgb;
	vec3 diffuse = (light.diffuse * diff * texture(_MainTex, v2f.TexCoords) * material.diffuse).rgb;
	float matSpecular = texture(_SpecTex, v2f.TexCoords).r;
	vec3 specular = (light.specular * spec * matSpecular).rgb;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	float shadow = CalcPointShadow(light);
	return ambient + shadow * (diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(Light light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - v2f.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	//sampling and calculate normal from texture
	vec3 normal = texture(_NormalTex, v2f.TexCoords).rgb;
	normal = normalize(normal*2.0- 1.0);
	normal = normalize(v2f.TBN * normal);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
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
	float matSpecular = texture(_SpecTex, v2f.TexCoords).r;
	vec3 specular = (light.specular * spec * matSpecular).rgb;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	float shadow = CalcDirectShadow(light);
	return ambient + shadow * (diffuse + specular);
}