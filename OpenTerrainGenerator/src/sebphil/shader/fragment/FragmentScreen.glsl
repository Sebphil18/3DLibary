#version 330 core

out vec4 color;

in VertexData {
	float testUniform;
	vec3 color;
} vertexIn;

void main() {
	color = vec4(vertexIn.color, 1.0);
}