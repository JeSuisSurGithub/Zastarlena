#ifndef PLANETGROUP_HPP
#define PLANETGROUP_HPP

#include "yzrendergroup.hpp"
#include "yzubo.hpp"
#include "rendergroups/stargroup.hpp"
#include <memory>

namespace yz
{
    namespace rendergroups
    {
        typedef struct ubo_material
        {
            alignas(16) glm::vec3 material_ambient;
            alignas(16) glm::vec3 material_diffuse;
            alignas(16) glm::vec3 material_specular;
            float shininess;
        }ubo_material;

        typedef struct planet
        {
            object base;
            ubo_material material;
            float planet_distance_to_star;
            float planet_revolution_speed;
            float planet_orbital_speed;
            float cur_angle;

            planet(
                rendergroup& group,
                const std::string& model_path,
                const std::string& texture_path,
                const std::string& height_map_path,
                glm::vec3 position,
                glm::vec3 rotation,
                glm::vec3 scale,
                ubo_material material_,
                float planet_distance_to_star,
                float planet_revolution_speed,
                float planet_orbital_speed,
                float cur_angle);
            ~planet();
        }planet;

        typedef struct planetgroup
        {
            planetgroup(const planetgroup &) = delete;
            planetgroup &operator=(const planetgroup &) = delete;
            planetgroup(planetgroup &&) = delete;
            planetgroup &operator=(planetgroup &&) = delete;

            std::unique_ptr<rendergroup> m_base;

            std::vector<planet> m_planets;

            memory::ubo m_ubo;

            planetgroup();
            ~planetgroup();
        }planetgroup;

        void update(planetgroup& context, float delta_time, const std::vector<star>& stars);
        void render(planetgroup& context, glm::vec3 camera_xyz);
    }
}

#endif /* PLANETGROUP_HPP */