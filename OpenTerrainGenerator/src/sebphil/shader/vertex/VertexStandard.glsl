#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;
layout(location = 3) in vec3 vBiTangent;
layout(location = 4) in vec2 texCoord;

uniform vec3 viewPos;
uniform vec3 lightPos;

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
	vec3 tbnPosition;
	vec3 tbnViewPos;
	vec3 tbnLightPos;
	vec3 viewPos;
	mat3 tbnMatrix;
} vertexOut;

void main() {

	vec4 worldPos = matrices.worldMat * vec4(vPosition, 1);
	vertexOut.position = worldPos.xyz;

	vec4 position = matrices.projectionMat * matrices.viewMat * worldPos;
	gl_Position = position;

	vec3 normal = mat3(matrices.normalMat) * vNormal;
	vertexOut.normal = normal;

	vec3 T = normalize((matrices.worldMat * vec4(vTangent, 0))).xyz;
	vec3 N = normalize((matrices.worldMat * vec4(vNormal, 0))).xyz;
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize((matrices.worldMat * vec4(vBiTangent, 0))).xyz;
	mat3 tbnMat = mat3(T, B, N);

	mat3 inverseTbnMat = transpose(tbnMat);

	vertexOut.tbnPosition = inverseTbnMat * worldPos.xyz;
	vertexOut.tbnViewPos = inverseTbnMat * viewPos;
	vertexOut.tbnLightPos = inverseTbnMat * lightPos;
	vertexOut.texCoord = texCoord;
	vertexOut.viewPos = viewPos;
	vertexOut.tbnMatrix = tbnMat;
}