#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

out vec3 fLocalPos;
out vec3 fNormal;

uniform mat4 view;
uniform mat4 projection;

void main() {

    fLocalPos = vPosition;
    fNormal = vNormal;

    mat4 fixedView = mat4(mat3(view));
    vec4 ndcPosition = projection * fixedView * vec4(vPosition, 1);

    gl_Position = ndcPosition.xyww;
}