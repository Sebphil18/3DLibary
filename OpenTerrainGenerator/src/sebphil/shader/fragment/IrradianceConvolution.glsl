#version 460 core
#define PI 3.14159

uniform samplerCubeArray envMap;

in vec3 fLocalPos;
in vec3 fNormal;
out vec4 color;

// TODO: https://learnopengl.com/PBR/IBL/Diffuse-irradiance
void main() {
	
	vec3 irradiance = vec3(0);

	vec3 normal = normalize(fLocalPos);
	vec3 right = normalize(cross(vec3(0, 1, 0), normal));
	vec3 up = normalize(cross(normal, right));
	
	float sampleStep = 0.025;
	float sampleCount = 0;

	for(float phi = 0; phi < 2 * PI; phi += sampleStep) {
		for(float theta = 0; theta < 0.5 * PI; theta += sampleStep) {

			// convert spherical coords to cartesian
			vec3 tangent = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));

			// tangent-space to world-space
			vec3 sampleVec = tangent.x * right + tangent.y * up + tangent.z * normal; 

			// cos: lamberts rule
			// sin: smaller areas on top of hemisphere
			irradiance += texture(envMap, vec4(sampleVec, 0)).rgb * cos(theta) * sin(theta);
			sampleCount++;
		}
	}

	// normalize
	irradiance = PI * irradiance * (1 / float(sampleCount));

	color = vec4(irradiance, 1);
	//color = texture(envMap, fLocalPos);
	//color = vec4(1, 0, 0, 1);
}