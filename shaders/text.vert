#version 460 core

layout (location = 0) in vec2 in_xy;
layout (location = 1) in vec2 in_uv;
layout (location = 0) out vec2 out_uv;

layout (location = 34) uniform vec2 translations;
layout (location = 38) uniform vec2 resolution;

vec2 convert_to_ogl_space(vec2 xy)
{
    vec2 screen_xy = (xy - (resolution / 2.0)) / (resolution / 2.0);
    return vec2(screen_xy.x, -screen_xy.y);
}

void main()
{
    gl_Position = vec4(convert_to_ogl_space(in_xy + translations), 0.0, 1.0);
    out_uv = in_uv;
}