#ifndef YZCOMMON_HPP
#define YZCOMMON_HPP

#define YZ_LOAD_SPIRV true

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
    typedef signed long long int isz;
    typedef std::size_t usz;

    const std::string WINDOW_NAME{"YuZhou"};
    constexpr u32 MAX_TEXTURE_COUNT{32};
    constexpr u32 MAX_POINT_LIGHT{32};
    constexpr float ZFAR{100000000.0};

    typedef enum UBO_BINDINGS
    {
        SHARED = 0,
        PLANET = 1,
        STAR = 2,
    }UBO_BINDINGS;

    typedef enum UNIFORM_LOCATIONS
    {
        TEXTURE = 0,
        CHARACTER_TEXTURE = 33,
        CHARACTER_TRANSLATION = 34,
        COMBINE_MAIN_SCENE = 35,
        COMBINE_BLOOM = 36,
        UPSAMPLE_TEXTURE = 37,
        SCREEN_RESOLUTION = 38,
        DOWNSAMPLE_TEXTURE = 39,
        SCREEN_TEARING_SCAN_POS = 40,
    }UNIFORM_LOCATIONS;

    typedef struct vertex
    {
        glm::vec3 xyz;
        glm::vec3 normal;
        glm::vec2 uv;
        bool operator==(const struct vertex& cmp) const
        {
            return (xyz == cmp.xyz && normal == cmp.normal && uv == cmp.uv);
        }
    }vertex;

    typedef struct ubo_point_light
    {
        alignas(16) glm::vec3 position;
        alignas(16) glm::vec3 range;
        alignas(16) glm::vec3 color;
    }ubo_point_light;

    typedef struct ubo_shared
    {
        alignas(0) glm::mat4 view;
        alignas(0) glm::mat4 projection;
        alignas(16) glm::vec3 camera_xyz;
        alignas(0) ubo_point_light point_lights[MAX_POINT_LIGHT];
        alignas(0) GLuint point_light_count;
    }ubo_shared;
}

#endif /* YZCOMMON_HPP */