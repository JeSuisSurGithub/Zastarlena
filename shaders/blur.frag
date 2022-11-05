#version 450 core

layout (location = 0) in vec2 in_uv;
layout (location = 0) out vec4 out_rgb;

layout (location = 138) uniform sampler2D in_image;
layout (location = 139) uniform bool horizontal;
const float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(in_image, 0);
    vec3 result = texture(in_image, in_uv).rgb * weight[0];
    if (horizontal)
    {
        for (int i = 1; i < 5; i++)
        {
           result += texture(in_image, in_uv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
           result += texture(in_image, in_uv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; i++)
        {
            result += texture(in_image, in_uv + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(in_image, in_uv - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    out_rgb = vec4(result, 1.0);
}

