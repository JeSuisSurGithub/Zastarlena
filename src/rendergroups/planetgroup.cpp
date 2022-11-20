#include "yzrendergroup.hpp"
#include <rendergroups/planetgroup.hpp>

namespace yz
{

namespace rendergroups
{
    planet::planet(
        rendergroup& group,
        const std::string& model_path,
        const std::string& texture_path,
        const std::string& height_map_path,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        ubo_material material_,
        float planet_distance_to_star_,
        float planet_revolution_speed_,
        float planet_orbital_speed_,
        float cur_angle_)
    :
    base(create_object(group, model_path, texture_path, height_map_path))
    {
        base.m_translation = position;
        base.m_euler_angles = rotation;
        base.m_scale = scale;
        material = material_;
        planet_distance_to_star = planet_distance_to_star_;
        planet_revolution_speed = planet_revolution_speed_;
        planet_orbital_speed = planet_orbital_speed_;
        cur_angle = cur_angle_;
    }

    planet::~planet() {}

    planetgroup::planetgroup()
    :
    m_ubo(1, nullptr, sizeof(ubo_material))
    {
        m_base = std::make_unique<rendergroup>("shaders/planets.vert", "shaders/planets.frag");
    }

    planetgroup::~planetgroup() {}

    void update(planetgroup& context, float delta_time, const std::vector<star>& stars)
    {
        usz planet_count = 0;
        for (usz star_count = 0; star_count < stars.size(); star_count++)
        {
            glm::vec3 position = stars[star_count].base.m_translation;
            for (usz index = 0; index < stars[star_count].planet_count; index++, planet_count++)
            {
                planet& shorthand = context.m_planets[planet_count];

                context.m_planets[planet_count].cur_angle += delta_time * shorthand.planet_orbital_speed;
                rotate(context.m_planets[planet_count].base,
                    glm::vec3(0.0, delta_time * shorthand.planet_revolution_speed, 0.0));
                shorthand.base.m_translation = {
                    position.x + glm::cos(shorthand.cur_angle) * shorthand.planet_distance_to_star,
                    0.0,
                    position.z + glm::sin(shorthand.cur_angle) * shorthand.planet_distance_to_star};
            }
        }
    }

    void render(planetgroup& context, glm::vec3 camera_xyz)
    {
        bind(*context.m_base->m_program);
        for (const planet& cur_object : context.m_planets)
        {
            if (glm::distance(camera_xyz, cur_object.base.m_translation) > ZFAR * 0.65)
                continue;
            bind(*context.m_base->m_textures[cur_object.base.m_texture_index]);
            glm::mat4 transform = get_transform_mat(cur_object.base);
            glm::mat4 inverse_transform = glm::inverse(transform);
            update_uint(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_INDEX, cur_object.base.m_texture_index);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::TRANSFORM, transform);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::INVERSE_TRANSFORM, inverse_transform);
            update(context.m_ubo, (void*)&cur_object.material, sizeof(ubo_material), 0);
            draw(*context.m_base->m_models[cur_object.base.m_model_index]);
        }
    }
}

}
