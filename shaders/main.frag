#version 460 core

layout(location = 1) in vec3 in_rgb;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec3 in_world_normal;
layout(location = 4) in vec3 in_world_xyz;

layout(location = 0) out vec4 out_rgb;
layout(location = 1) out vec4 out_bright_rgb;

layout(location = 0) uniform sampler2D in_texture[32];
layout(location = 32) uniform int texture_index;
layout(location = 38) uniform vec3 camera_xyz;
layout(location = 140) uniform float texture_offset;

const vec3 DIRECTION = vec3(0.0, -1.0, 0.0);
const float AMBIENT_STRENGTH = 0.1;

#define MAX_POINT_LIGHT 32
layout(location = 39) uniform vec3 position[MAX_POINT_LIGHT];
layout(location = 71) uniform vec3 range_constants[MAX_POINT_LIGHT];
layout(location = 103) uniform vec3 color[MAX_POINT_LIGHT];
layout(location = 135) uniform int current_point_light_count;

vec3 calc_directional(vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-DIRECTION);

    float diffuse = max(dot(normal, light_direction), 0.0);

    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular = pow(max(dot(view_direction, reflect_direction), 0.0), 2);

    return vec3(AMBIENT_STRENGTH + diffuse + specular);
}

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

    vec3 result = vec3(1.0, 1.0, 1.0); //calc_directional(normal, view_direction);

    for (int i = 0; i < current_point_light_count; i++)
        result += calc_point_light(position[i], range_constants[i], color[i], normal, in_world_xyz, view_direction);

    vec3 texture_color = texture(in_texture[texture_index], vec2(in_uv.s, in_uv.t + texture_offset)).rgb * 2.0;
    vec4 hdr_color = vec4(result * texture_color, 1.0);
    out_rgb = hdr_color;

    float brightness = dot(out_rgb.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        out_bright_rgb = vec4(out_rgb.rgb, 1.0);
    else
        out_bright_rgb = vec4(0.0, 0.0, 0.0, 1.0);
}