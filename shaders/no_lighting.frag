#version 450 core

layout(location = 1) in vec3 in_rgb;
layout(location = 2) in vec2 in_uv;

layout(location = 0) out vec4 out_rgb;
layout(location = 0) uniform sampler2D in_texture[32];
layout(location = 32) uniform int texture_index;

void main()
{
    out_rgb = texture(in_texture[texture_index], in_uv);
}