#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 4) in vec2 texCoord;

uniform float testUni;

out VertexData {
	float testUniform;
	vec3 color;
	vec2 texCoord;
} vertexOut;

void main() {

	gl_Position = vec4(vPosition, 1);

	vertexOut.testUniform = testUni;
	vertexOut.texCoord = texCoord;
}