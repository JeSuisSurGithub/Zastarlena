#version 460 core

layout(location = 1) in vec3 in_rgb;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec3 in_world_normal;
layout(location = 4) in vec3 in_world_xyz;

layout(location = 0) out vec4 out_rgb;
layout(location = 1) out vec4 out_bright_rgb;

#define MAX_POINT_LIGHT 32
#define MAX_TEXTURE_COUNT 32

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
    point_light point_lights[MAX_POINT_LIGHT];
    uint current_point_light_count;
};

layout(location = 0) uniform sampler2D textures[MAX_TEXTURE_COUNT];
layout(location = 32) uniform uint texture_index;
layout(location = 40) uniform float texture_offset;

const float AMBIENT_STRENGTH = 0.1;

vec3 calc_point_light(vec3 position, vec3 range, vec3 color, vec3 normal, vec3 frag_xyz, vec3 view_direction)
{
    vec3 light_direction = normalize(position - frag_xyz);

    float diffuse = max(dot(normal, light_direction), 0.0);

    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular = pow(max(dot(view_direction, reflect_direction), 0.0), 2);

    float distance = length(position - frag_xyz);
    float attenuation = 1.0 / (range.x + range.y * distance +
  		range.z * (distance * distance));

    vec3 ambient = AMBIENT_STRENGTH * color * attenuation;
    vec3 diffusev = vec3(diffuse * attenuation);
    vec3 specularv = vec3(specular * attenuation);
    return (ambient + diffusev /* + specularv */);
}

void main()
{
    vec3 normal = normalize(in_world_normal);
    vec3 view_direction = normalize(camera_xyz - in_world_xyz);

    vec3 lighting = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < current_point_light_count; i++)
        lighting +=
            calc_point_light(point_lights[i].position, point_lights[i].range, point_lights[i].color, normal, in_world_xyz, view_direction);

    vec3 texture_color = texture(textures[texture_index], vec2(in_uv.s, in_uv.t + texture_offset)).rgb * 2.0;
    vec4 hdr_color = vec4(5.0 * lighting * texture_color, 1.0);
    out_rgb = hdr_color;

    float brightness = dot(out_rgb.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        out_bright_rgb = vec4(out_rgb.rgb, 1.0);
    else
        out_bright_rgb = vec4(0.0, 0.0, 0.0, 1.0);
}