#version 460 core
#define PI 3.14159
#define RESOLUTION 256

uniform samplerCubeArray envMap;

in vec3 fLocalPos;
out vec4 color;

uniform float roughness;

// source: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html 04/22/2021

// bitshift magic to mirror number around decimal point :)
float radicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley2d(uint i, uint n) {
    return vec2(float(i)/float(n), radicalInverse_VdC(i));
}

vec3 importanceSampleNDF(vec2 xi, vec3 normal, float roughness);
float distributionGGX(vec3 normal, vec3 halfway, float roughness);

void main() {

	// assumption/approximation that all are equal
	vec3 normal = normalize(fLocalPos);
	vec3 reflectionDir = normal;
	vec3 viewDir = normal;

	const uint sampleCount = 1024u;

	float totalWeight = 0;
	vec3 irradiance = vec3(0);

	for(uint i = 0; i < sampleCount; i++) {
		
		vec2 xi = hammersley2d(i, sampleCount);
		vec3 halfway = importanceSampleNDF(xi, normal, roughness);
		vec3 lightDir = normalize(2 * dot(viewDir, halfway) * halfway - viewDir);

		float NdotL = max(dot(normal, lightDir), 0);

		if(NdotL > 0) {

			float NdotH = max(dot(normal, halfway), 0);
			float HdotV = max(dot(halfway, viewDir), 0);

			float distribution = distributionGGX(normal, halfway, roughness);
			float pdf = (distribution * NdotH / (4 * HdotV)) + 0.0001;

			float saTexel = 4 * PI / (6.0 * RESOLUTION * RESOLUTION);
			float saSample = 1 / (float(sampleCount) * pdf + 0.0001);

			float level = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 

			irradiance += textureLod(envMap, vec4(lightDir, 0), roughness).rgb * NdotL;
			totalWeight += NdotL;
		}
	}
	
	irradiance /= totalWeight;

	color = vec4(irradiance, 1);
}

vec3 importanceSampleNDF(vec2 xi, vec3 normal, float roughness) {

	float a = roughness * roughness;

//    float cosTheta = (1 - xi.y) / (1 + (a * a - 1) * xi.y);
//    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	float cosTheta = sqrt((1.0 - xi.y) / (1.0 + (a*a - 1.0) * xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	float phi = 2 * PI * xi.x;

	// spherical to Cartesian
	vec3 sphericalVec = vec3(0);
	sphericalVec.x = cos(phi) * sinTheta;
	sphericalVec.y = sin(phi) * sinTheta;
	sphericalVec.z = cosTheta;

	// tangent-space to world space
	vec3 up = abs(normal.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 tangent = normalize(cross(up, normal));
	vec3 bitangent = cross(normal, tangent);

	vec3 sampleVec = tangent * sphericalVec.x + bitangent * sphericalVec.y + normal * sphericalVec.z;

	return normalize(sampleVec);
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