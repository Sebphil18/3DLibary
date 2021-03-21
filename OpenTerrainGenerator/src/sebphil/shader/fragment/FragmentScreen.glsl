#version 460 core

out vec4 color;

struct Material {
	sampler2D albedoTex0;
};

uniform Material material;

in VertexData {
	float testUniform;
	vec3 color;
	vec2 texCoord;
} vertexIn;

void main() {

	vec4 texColor = texture(material.albedoTex0, vec2(vertexIn.texCoord.x, 1 - vertexIn.texCoord.y));

	color = texColor * vec4(vertexIn.color, 1);
}