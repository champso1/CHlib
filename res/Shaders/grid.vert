#version 330 core

layout (location = 0) in vec2 vs_pos;
layout (location = 1) in vec2 vs_uv;
out vec2 uv;

void main() {
	 gl_Position = vec4(vs_pos, 0.0, 1.0);
	 uv = vs_uv;
}