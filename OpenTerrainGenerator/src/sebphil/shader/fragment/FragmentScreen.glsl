#version 330 core

out vec4 color;

uniform sampler2D tex;

in VertexData {
	float testUniform;
	vec3 color;
	vec2 texCoord;
} vertexIn;

void main() {
	vec4 texColor = texture(tex, vertexIn.texCoord * 2);
	color = texColor;
}