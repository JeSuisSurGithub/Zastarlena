#version 460 core

layout(location = 1) in vec3 in_rgb;
layout(location = 2) in vec2 in_uv;

layout(location = 0) out vec4 out_rgb;

layout(location = 0) uniform sampler2D textures[32];
layout(location = 32) uniform uint texture_index;

void main()
{
    out_rgb = texture(textures[texture_index], in_uv);
}