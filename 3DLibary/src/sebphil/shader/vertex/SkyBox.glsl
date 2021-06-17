#version 460 core

layout(location = 0) in vec3 vPosition;

out vec3 localPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    localPos = vPosition;

    mat4 fixedView = mat4(mat3(view));
    vec4 ndcPosition = projection * fixedView * vec4(vPosition, 1);

    gl_Position = ndcPosition.xyww;
}