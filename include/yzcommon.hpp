#ifndef YZCOMMON_HPP
#define YZCOMMON_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>

#include <cstdint>
#include <iostream>
#include <string>

namespace yz
{
    typedef std::int32_t i32;
    typedef std::uint8_t u8;
    typedef std::uint32_t u32;

    typedef struct vertex_
    {
        glm::vec3 xyz;
        glm::vec3 rgb;
        glm::vec2 uv;
        glm::vec3 normal;
        bool operator==(const struct vertex_& cmp) const
        {
            return (xyz == cmp.xyz && rgb == cmp.rgb && uv == cmp.uv && normal == cmp.normal);
        }
    }vertex;

    const std::string WINDOW_NAME{"YuZhou"};

    constexpr u32 MAX_TEXTURE_COUNT{32};
    constexpr u32 MAX_POINT_LIGHT{32};

    typedef enum UNIFORM_LOCATIONS_
    {
        TEXTURE = 0,
        TEXTURE_INDEX = 32,
        MODEL = 33,
        VIEW = 34,
        PROJECTION = 35,
        TRANSFORM = 36,
        INVERSE_TRANSFORM = 37,
        CAMERA_XYZ = 38,
        POINT_LIGHTS_POSITION = 39,
        POINT_LIGHTS_ATTENUATION_CONST = 71,
        POINT_LIGHTS_COLOR = 103,
        CURRENT_POINT_LIGHT_COUNT = 135,
        COMBINE_MAIN_SCENE = 136,
        COMBINE_BLURRED = 137,
        BLUR_INPUT_IMAGE = 138,
        BLUR_HORIZONTAL = 139,
        TEXTURE_SCROLL_OFFSET = 140
    }UNIFORM_LOCATIONS;
}

#endif /* YZCOMMON_HPP */