#ifndef PLANETGROUP_HPP
#define PLANETGROUP_HPP

#include "yzrendergroup.hpp"
#include <memory>

namespace yz
{
    namespace rendergroups
    {
        typedef struct ubo_planet
        {
            alignas(4) glm::vec3 material_ambient;
            alignas(4) glm::vec3 material_diffuse;
            alignas(4) glm::vec3 material_specular;
            alignas(4) float shininess;
        }ubo_planet;

        typedef struct planetgroup
        {
            planetgroup(const planetgroup &) = delete;
            planetgroup &operator=(const planetgroup &) = delete;
            planetgroup(planetgroup &&) = delete;
            planetgroup &operator=(planetgroup &&) = delete;

            std::unique_ptr<rendergroup> m_base;

            planetgroup();
            ~planetgroup();
        }planetgroup;

        void update(planetgroup& context, float delta_time);
        void render(planetgroup& context, glm::vec3 camera_xyz);
    }
}

#endif /* PLANETGROUP_HPP */