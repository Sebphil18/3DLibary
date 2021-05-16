#version 460 core

#define PI 3.14159265

out vec4 color;

struct Material {

	sampler2D albedoTex0;
	sampler2D roughnessTex0;
	sampler2D metallicTex0;
	sampler2D occlusionTex0;
	sampler2D normalTex0;

	vec3 albedoColor;

	float roughness;
	float metallic;
	float occlusion;
};

uniform Material material;

uniform samplerCubeArray envMap;

uniform samplerCubeArray prefilterMap;
uniform sampler2D brdf;

in VertexData {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec3 tbnPosition;
	vec3 tbnViewPos;
	vec3 tbnLightPos;
	vec3 viewPos;
	mat3 tbnMatrix;
} vertexIn;

float distributionGGX(vec3 normal, vec3 halfway, float roughness);
float geometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness);
float geometrySchlickGGX(vec3 normal, vec3 direction, float roughness);
vec3 fresnelSchlick(vec3 viewDir, vec3 halfway, vec3 f0);
vec3 fresnelSchlickRough(vec3 viewDir, vec3 halfway, vec3 f0, float roughness);

// TODO: normal-mapping
// viewDirection (viewPos, vertexIn.position)
// lightDir (lightPos, vertexIn.position)
// e.g. transform lightPos, viewPos and fragmentPos in vertex shader

void main() {

	const vec3 lightColor = vec3(1, 1, 1);
	//vec3 normal = normalize(vertexIn.normal);
	vec3 normal = texture(material.normalTex0, vertexIn.texCoord).rgb;
	normal = normalize(normal * 2 - 1);

	// standard
	vec3 texColor = texture(material.albedoTex0, vertexIn.texCoord).rgb;
	float texRoughness = texture(material.roughnessTex0, vertexIn.texCoord).r;
	float texMetallic = texture(material.metallicTex0, vertexIn.texCoord).r;
	float texOcclusion = texture(material.occlusionTex0, vertexIn.texCoord).r;

	// before loop
	vec3 albedo = texColor + material.albedoColor;
	float roughness = texRoughness + material.roughness;
	float metallic = texMetallic + material.metallic;
	float occlusion = texOcclusion + material.occlusion;

	 // DEBUG
//	albedo = vec3(0, 1, 1);
//	roughness = 0.2;
//	metallic = 0.99;
//	normal = vec3(0, 0, 1);
	
	occlusion = 1;

	vec3 viewDir = normalize(vertexIn.tbnViewPos - vertexIn.tbnPosition);
	vec3 L0 = vec3(0);

	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metallic);

	// in loop -----------------------------------------------------------
	vec3 lightDir = normalize(vertexIn.tbnLightPos - vertexIn.tbnPosition);
	vec3 halfway = normalize(lightDir + viewDir);

	// BDRF
	float dis = length(vertexIn.tbnLightPos - vertexIn.tbnPosition);
	float attentuation = 1.0 / (dis * dis);
	vec3 lightRadiance = lightColor * attentuation;
	vec3 radiance = lightRadiance;

	// determine active microfacets
	float D = distributionGGX(normal, halfway, roughness);
	float G = geometrySmith(normal, viewDir, lightDir, roughness);
	// calculate reflacted light
	vec3 F = fresnelSchlick(halfway, viewDir, f0);

	vec3 a = D * G * F;
	float b = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0);
	vec3 specular = a / max(b, 0.001);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	// diffuse light should be zero when material is metal
	kD *= 1.0 - metallic;

	float NdotL = max(dot(normal, lightDir), 0.0);

	L0 += (kD * albedo / PI + specular) * radiance * NdotL;
	// end of loop ---------------------------------------------------------------------------------------

	// IBL
	// diffuse

	// TODO: normal mapping for IBL does not work yet
	viewDir = normalize(vertexIn.viewPos - vertexIn.position);
	normal = normalize(vertexIn.tbnMatrix * normal);

	vec3 kSAmb = fresnelSchlickRough(normal, viewDir, f0, roughness);
	vec3 kDAmb = 1 - kSAmb;
	vec3 irradianceAmb = texture(envMap, vec4(normal, 0)).rgb;
	vec3 diffuseAmb = irradianceAmb * albedo;

	// specular
	vec3 reflectedDir = reflect(-viewDir, normal);
	const float maxLevel = 4;
	float level = roughness * maxLevel;
	vec3 prefilteredColor = textureLod(prefilterMap, vec4(reflectedDir, 0), level).rgb;

	vec2 brdfSampleVector = vec2(max(dot(normal, viewDir), 0), roughness);
	vec2 envBRDF = texture(brdf, brdfSampleVector).rg;

	vec3 specIBL = prefilteredColor * (kSAmb * envBRDF.x + envBRDF.y);

	// combine
	vec3 ambientColor = (kDAmb * diffuseAmb + specIBL) * occlusion;
	//ambientColor = vec3(0);

	color = vec4(L0 + ambientColor, 1);
	//color = texture(material.normalTex0, vertexIn.texCoord);
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
	vec3 result = f0 + (1 - f0) * pow(max(1 - reflectiveAngle, 0), 5);

	return result;
}

vec3 fresnelSchlickRough(vec3 viewDir, vec3 halfway, vec3 f0, float roughness) {
	
	/*
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
	*/

	// when angle between viewDir and halfway is 90° most light gets reflacted and when angle is 0° no Light gets reflacted back to the observer
	float reflectiveAngle = max(dot(halfway, viewDir), 0);
	vec3 result = f0 + (max(vec3(1 - roughness), f0) - f0) * pow(max(1 - reflectiveAngle, 0), 5);

	return result;
}