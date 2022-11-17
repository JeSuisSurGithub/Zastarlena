#ifndef STARGROUP_HPP
#define STARGROUP_HPP

#include "yzrendergroup.hpp"
#include <memory>

namespace yz
{
    namespace rendergroups
    {
        typedef struct stargroup
        {
            stargroup(const stargroup &) = delete;
            stargroup &operator=(const stargroup &) = delete;
            stargroup(stargroup &&) = delete;
            stargroup &operator=(stargroup &&) = delete;

            std::unique_ptr<rendergroup> m_base;
            float m_texture_offset_count;

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