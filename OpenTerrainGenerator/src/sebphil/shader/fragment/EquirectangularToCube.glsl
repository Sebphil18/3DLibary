#version 460 core

uniform sampler2D equirectangularMap;

in vec3 fLocalPos;
out vec4 color;

const vec2 inverseAtan = vec2(0.1591, 0.3183);

vec2 getTexCoord(vec3 direction);

void main() {

	vec2 texCoord = getTexCoord(normalize(fLocalPos));
	color = texture(equirectangularMap, texCoord);
}

vec2 getTexCoord(vec3 direction) {

	vec2 texCoord = vec2(atan(direction.z, direction.x), asin(direction.y));
	texCoord *= inverseAtan;
	texCoord += 0.5;

	return texCoord;
}