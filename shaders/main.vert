#version 450 core

layout (location = 0) in vec3 in_xyz;
layout (location = 1) in vec3 in_rgb;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_normal;

layout (location = 1) out vec3 out_rgb;
layout (location = 2) out vec2 out_uv;
layout (location = 3) out vec3 out_world_normal;
layout (location = 4) out vec3 out_world_xyz;

layout (location = 33) uniform mat4 model;
layout (location = 34) uniform mat4 view;
layout (location = 35) uniform mat4 projection;
layout (location = 36) uniform mat4 transform;
layout (location = 37) uniform mat4 inverse_transform;

void main()
{
    vec4 world_xyz = transform * vec4(in_xyz, 1.0);
    gl_Position = projection * view * model * world_xyz;

    out_rgb = in_rgb;
    out_uv = in_uv;
    out_world_normal = normalize(mat3(model) * mat3(transpose(inverse_transform)) * in_normal);
    out_world_xyz = vec3(model * world_xyz);
}