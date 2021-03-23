#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 4) in vec2 texCoord;

layout(std140) uniform Matrices {
	mat4 worldMat;
	mat4 normalMat;
	mat4 viewMat;
	mat4 projectionMat;
} matrices;

out VertexData {
	vec3 color;
	vec2 texCoord;
} vertexOut;

void main() {

	vec4 position = matrices.projectionMat * matrices.viewMat * matrices.worldMat * vec4(vPosition, 1);

	gl_Position = position;

	vertexOut.texCoord = texCoord;
}