#version 460 core

out vec4 color;
in vec3 localPos;

uniform samplerCubeArray cubeMap;

void main() {
	
	vec4 texColor = texture(cubeMap, vec4(localPos, 0));
	color = texColor;
}