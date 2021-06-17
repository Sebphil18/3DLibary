#version 460 core

layout(location = 0) in vec3 vPosition;

out vec2 fLocalPos;

void main() {
    
    vec2 mappedPos = vec2(vPosition.x, vPosition.y);
    mappedPos.x += 1;
    mappedPos.y += 1;
    mappedPos /= 2;

    fLocalPos = mappedPos;

    gl_Position = vec4(vPosition, 1);
}