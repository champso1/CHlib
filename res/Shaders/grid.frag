#version 330 core

in vec2 uv;
out vec4 color;

uniform sampler2D tex_sampler;

void main() {
	 color = texture(tex_sampler, uv);
}
