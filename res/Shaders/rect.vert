#version 430 core

layout (location = 0) in vec3 in_color;
layout (location = 1) in vec2 in_pos;

layout (location = 10) in mat4 transformation;

out vec3 vs_color;

void main() {
	 gl_Position = transformation * vec4(in_pos, 0.0, 1.0);
	 vs_color = in_color;
}
