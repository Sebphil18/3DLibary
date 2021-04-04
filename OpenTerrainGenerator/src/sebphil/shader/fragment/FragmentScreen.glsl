#version 460 core

out vec4 color;

uniform float exposure = 1;

struct Material {
	sampler2D albedoTex0;
};

uniform Material material;

in VertexData {
	vec2 texCoord;
} vertexIn;

void main() {

	vec4 texColor = texture(material.albedoTex0, vertexIn.texCoord);
	
	// HDR
	vec4 mapped = vec4(1) - exp(-texColor * exposure);

	// Gamma-Correction
	const float gamma = 2.2;
	mapped = pow(mapped, vec4(1 / gamma));

	color = mapped;
}