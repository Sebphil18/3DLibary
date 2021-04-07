#version 460 core

#define PI 3.14159265

out vec4 color;

struct Material {

	sampler2D albedoTex0;
	sampler2D roughnessTex0;
	sampler2D metallicTex0;

	vec3 albedoColor;

	float roughness;
	float metallic;
};

uniform Material material;
uniform vec3 viewPos;
uniform vec3 lightPos;

in VertexData {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} vertexIn;

float distributionGGX(vec3 normal, vec3 halfway, float roughness);
float geometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness);
float geometrySchlickGGX(vec3 normal, vec3 direction, float roughness);
vec3 fresnelSchlick(vec3 viewDir, vec3 halfway, vec3 f0);

void main() {

	const vec3 lightColor = vec3(1, 1, 1);
	vec3 normal = normalize(vertexIn.normal);

	// standard
	vec3 texColor = texture(material.albedoTex0, vertexIn.texCoord).rgb;
	float texRoughness = texture(material.roughnessTex0, vertexIn.texCoord).r;
	float texMetallic = texture(material.metallicTex0, vertexIn.texCoord).r;

	// before loop
	vec3 albedo = texColor + material.albedoColor;
	float roughness = texRoughness + material.roughness;
	float metallic = texMetallic + material.metallic;

	vec3 viewDir = normalize(viewPos - vertexIn.position);
	vec3 L0 = vec3(0);

	// in loop
	vec3 lightDir = normalize(lightPos - vertexIn.position);
	vec3 halfway = normalize(lightDir + viewDir);

	float dis = length(lightPos - vertexIn.position);
	float attentuation = 1.0 / (dis * dis);
	vec3 radiance = lightColor * attentuation;

	// BDRF
	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metallic);

	// determine active microfacets
	float D = distributionGGX(normal, halfway, roughness);
	float G = geometrySmith(normal, viewDir, lightDir, roughness);
	// calculate reflacted light part
	vec3 F = fresnelSchlick(viewDir, halfway, f0);

	vec3 a = D * G * F;
	float b = 4.0 * max(dot(normal, viewDir), 0.0001) * max(dot(normal, lightDir), 0.0001);
	vec3 specular = a / b;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	// diffuse light should be zero when material is metal
	kD *= 1.0 - metallic;

	float NdotL = max(dot(normal, lightDir), 0.0);

	L0 += (kD * albedo / PI + specular) * radiance * NdotL;

	color = vec4(L0 + albedo * 0.004, 1);
}

float distributionGGX(vec3 normal, vec3 halfway, float roughness) {

    float r = roughness * roughness;
    float r2 = r*r;
    float angle = max(dot(normal, halfway), 0.0);
    float angle2 = angle*angle;
	
    float a = r2;
    float b = (angle2 * (r2 - 1.0) + 1.0);
    b = PI * b * b;
	
    return a / b;
}

float geometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness) {
	
	// approximate obstructed area of microfactes
    float gObstructed = geometrySchlickGGX(normal, viewDir, roughness);
    // approximate overshadowed area of microfactes
	float gShadowed = geometrySchlickGGX(normal, lightDir, roughness);
	
    return gObstructed * gShadowed;
}

float geometrySchlickGGX(vec3 normal, vec3 direction, float roughness) {

	float angle = max(dot(normal, direction), 0.0);

	// remap roughness
    float r = roughness + 1.0;
    float kDirect = (r*r) / 8.0;

    float a = angle;
    float b = angle * (1.0 - kDirect) + kDirect;
	
    return a / b;
}


vec3 fresnelSchlick(vec3 viewDir, vec3 halfway, vec3 f0) {

	// when angle between viewDir and halfway is 90° most light gets reflacted and when angle is 0° no Light gets reflacted back to the observer
	float reflectiveAngle = max(dot(halfway, viewDir), 0);
	return f0 + (1 - f0) * pow(max(1 - reflectiveAngle, 0), 5);
}