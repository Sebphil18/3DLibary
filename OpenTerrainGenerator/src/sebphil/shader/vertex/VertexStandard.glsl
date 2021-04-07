#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;
layout(location = 3) in vec3 vBiTangent;
layout(location = 4) in vec2 texCoord;

layout(std140) uniform Matrices {
	mat4 worldMat;
	mat4 normalMat;
	mat4 viewMat;
	mat4 projectionMat;
} matrices;

out VertexData {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} vertexOut;

void main() {

	vec4 position = matrices.projectionMat * matrices.viewMat * matrices.worldMat * vec4(vPosition, 1);

	gl_Position = position;

	vertexOut.position = vec3(matrices.worldMat * vec4(vPosition, 1));

	vec3 normal = mat3(matrices.normalMat) * vNormal;
	vertexOut.normal = normal;

	vertexOut.texCoord = texCoord;
}