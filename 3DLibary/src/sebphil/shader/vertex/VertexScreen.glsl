#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 4) in vec2 texCoord;

out VertexData {
	vec2 texCoord;
} vertexOut;

void main() {

	gl_Position = vec4(vPosition, 1);

	vertexOut.texCoord = texCoord;
}