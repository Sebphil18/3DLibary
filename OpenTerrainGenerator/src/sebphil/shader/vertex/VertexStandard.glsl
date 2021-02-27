#version 460 core

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec3 vColor;

uniform float testUni;

out VertexData {
	float testUniform;
	vec3 color;
} vertexOut;

void main() {
	gl_Position = vec4(vPosition.x, vPosition.y, 0, 1);
	vertexOut.testUniform = testUni;
	vertexOut.color = vColor;
}