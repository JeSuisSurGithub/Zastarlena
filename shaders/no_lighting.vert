#version 460 core

layout (location = 0) in vec3 in_xyz;
layout (location = 1) in vec3 in_rgb;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_normal;

layout (location = 1) out vec3 out_rgb;
layout (location = 2) out vec2 out_uv;

struct point_light
{
    vec3 position;
    vec3 range;
    vec3 color;
};

layout (std140, binding = 0) uniform shared_ubo
{
    mat4 model;
    mat4 view;
    mat4 projection;
    vec3 camera_xyz;
    point_light point_lights[32];
    uint current_point_light_count;
};

layout (location = 33) uniform mat4 transform;

void main()
{
    vec4 world_xyz = transform * vec4(in_xyz, 1.0);
    gl_Position = projection * view * model * world_xyz;

    out_rgb = in_rgb;
    out_uv = in_uv;
}