#version 460 core

layout (location = 0) in vec2 in_uv;
layout (location = 0) out vec4 out_rgb;

layout (location = 136) uniform sampler2D main_image;
layout (location = 137) uniform sampler2D bloom;

void main()
{
    const float GAMMA = 1.0;
    const float EXPOSURE = 0.5;
    vec3 hdr_color = texture(main_image, in_uv).rgb;
    vec3 bloom_color = texture(bloom, in_uv).rgb;
    hdr_color = hdr_color + bloom_color;

    vec3 tone_mapped = vec3(1.0) - exp(-hdr_color * EXPOSURE);

    tone_mapped = pow(tone_mapped, vec3(1.0 / GAMMA));
    out_rgb = vec4(tone_mapped, texture(main_image, in_uv).a);
}