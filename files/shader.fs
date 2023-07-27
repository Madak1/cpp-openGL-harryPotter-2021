#version 400 core

out vec4 FragColor;
in vec2 vtf_TexCoord;

uniform sampler2D u_texture;

void main() {
    FragColor = texture(u_texture, vtf_TexCoord);
}
