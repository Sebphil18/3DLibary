#version 460 core
#define PI 3.14159

in vec2 fLocalPos;
out vec4 color;

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

vec2 integrateBRDF(float NdotV, float roughness);
vec3 importanceSampleNDF(vec2 xi, vec3 normal, float roughness);
float geometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness);
float geometrySchlickGGX(vec3 normal, vec3 direction, float roughness);

void main() {
	
	vec2 integratedBRDF = integrateBRDF(fLocalPos.x, fLocalPos.y);
	color = vec4(integratedBRDF, 0, 1);
}

vec2 integrateBRDF(float NdotV, float roughness) {
	
	vec3 normal = vec3(0, 0, 1);
	vec3 viewDir = vec3(0);
	viewDir.x = sqrt(1 - NdotV * NdotV);
	viewDir.y = 0;
	viewDir.z = NdotV;

	// integrate with Monte-Carlo
	const uint sampleCount = 2048;
	float x = 0;
	float y = 0;

	for(uint i = 0; i < sampleCount; i++) {

		vec2 xi = hammersley2d(i, sampleCount);
		vec3 halfway = importanceSampleNDF(xi, normal, roughness);
		vec3 lightDir = normalize(2 * dot(viewDir, halfway) * halfway - viewDir);

		float NdotL = max(dot(normal, lightDir), 0);
		float NdotH = max(dot(normal, halfway), 0);
		float HdotV = max(dot(halfway, viewDir), 0);
		float VdotH = max(dot(viewDir, halfway), 0);

		if(NdotL > 0) {
			
			float geometry = geometrySmith(normal, viewDir, lightDir, roughness);
			float geoemtryVis = (geometry * VdotH) / (NdotH * NdotV);
			float Fc = pow(1 - VdotH, 5);

			x += (1 - Fc) * geoemtryVis;
			y += Fc * geoemtryVis;

		}
	}
	
	x /= sampleCount;
	y /= sampleCount;

	return vec2(x, y);
}

vec3 importanceSampleNDF(vec2 xi, vec3 normal, float roughness) {

	float a = roughness * roughness;

    float cosTheta = (1 - xi.y) / (1 + (a * a - 1) * xi.y);
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
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

float geometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness) {
	
	float obscured = geometrySchlickGGX(normal, viewDir, roughness);
	float shadowed = geometrySchlickGGX(normal, lightDir, roughness);

	return obscured * shadowed;
}

float geometrySchlickGGX(vec3 normal, vec3 direction, float roughness) {

	float angle = max(dot(normal, direction), 0.0);

	// remap roughness
    float r = roughness;

    float kIBL = (r*r) / 2;

    float a = angle;
    float b = angle * (1.0 - kIBL) + kIBL;
	
    return a / b;
}