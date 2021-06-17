#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

in vec3 gLocalPos[];
out vec3 fLocalPos;

uniform mat4 projection;
uniform mat4 view[6];

void main() {

	for(int layer = 0; layer < 6; layer++) {
		
		gl_Layer = layer;

		for(int vertex = 0; vertex < 3; vertex++) {
			
			gl_Position = projection * view[layer] * vec4(gLocalPos[vertex], 1);
			fLocalPos = gLocalPos[vertex];

			EmitVertex();
		}
	}

	EndPrimitive();
}