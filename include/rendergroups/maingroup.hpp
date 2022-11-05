#ifndef MAINGROUP_HPP
#define MAINGROUP_HPP

#include "yzcommon.hpp"
#include "yzrendergroup.hpp"

#include <memory>

namespace yz
{
    namespace rendergroups
    {
        constexpr std::array<glm::vec3, 12> RANGES =
        {{
            glm::vec3(1.f, 0.7f, 1.8),
            glm::vec3(1.f, 0.35f, 0.44),
            glm::vec3(1.f, 0.22f, 0.20),
            glm::vec3(1.f, 0.14f, 0.07),
            glm::vec3(1.f, 0.09f, 0.032),
            glm::vec3(1.f, 0.07f, 0.017),
            glm::vec3(1.f, 0.045f, 0.0075),
            glm::vec3(1.f, 0.027f, 0.0028),
            glm::vec3(1.f, 0.022f, 0.0019),
            glm::vec3(1.f, 0.014f, 0.0007),
            glm::vec3(1.f, 0.007f, 0.0002),
            glm::vec3(1.f, 0.0014f, 0.000007),
        }};

        typedef struct maingroup_
        {
            maingroup_(const maingroup_ &) = delete;
            maingroup_ &operator=(const maingroup_ &) = delete;
            maingroup_(maingroup_ &&) = delete;
            maingroup_ &operator=(maingroup_ &&) = delete;

            std::unique_ptr<rendergroup> m_base;
            std::array<glm::vec3, MAX_POINT_LIGHT> m_point_light_position;
            std::array<glm::vec3, MAX_POINT_LIGHT> m_point_light_range;
            std::array<glm::vec3, MAX_POINT_LIGHT> m_point_light_color;
            u32 m_current_point_light_count;
            float m_texture_offset_count;

            maingroup_();
            ~maingroup_();
        }maingroup;

        void update(maingroup& context, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camera_xyz, float delta_time);

        void render(maingroup& context);

        void add_point_light(maingroup& context, glm::vec3 position, glm::vec3 range, glm::vec3 color);
    }
}

#endif /* MAINGROUP_HPP */