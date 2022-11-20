#ifndef STARGROUP_HPP
#define STARGROUP_HPP

#include "yzrendergroup.hpp"
#include <memory>

namespace yz
{
    namespace rendergroups
    {
        typedef struct star
        {
            object base;
            ubo_point_light point_light;
            usz planet_count;

            star(
                rendergroup& group,
                const std::string& model_path,
                const std::string& texture_path,
                glm::vec3 position,
                glm::vec3 rotation,
                glm::vec3 scale,
                glm::vec3 color,
                glm::vec3 range,
                usz planet_count_);
            ~star();
        }star;

        typedef struct stargroup
        {
            stargroup(const stargroup &) = delete;
            stargroup &operator=(const stargroup &) = delete;
            stargroup(stargroup &&) = delete;
            stargroup &operator=(stargroup &&) = delete;

            std::unique_ptr<rendergroup> m_base;
            float m_texture_offset_count;

            std::vector<star> m_stars;

            stargroup();
            ~stargroup();
        }stargroup;

        void update(stargroup& context, float delta_time);
        void render(stargroup& context, glm::vec3 camera_xyz);

        // According to https://wiki.ogre3d.org/Light+Attenuation+Shortcut
        // Linear = 4.5 / LightRange
        // Quadratic = 75.0 / LightRange^2
        glm::vec3 light_range_constants(float lightrange);
    }
}

#endif /* STARGROUP_HPP */