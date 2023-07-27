#version 400 core
layout (location = 0) in vec3 in_Pos;
layout (location = 1) in vec2 in_TexCoord;

out vec2 vtf_TexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(in_Pos, 1.0);
    vtf_TexCoord = in_TexCoord;
}
