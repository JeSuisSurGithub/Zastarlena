#include <rendergroups/stargroup.hpp>

namespace yz
{

namespace rendergroups
{
    star::star(
        rendergroup& group,
        const std::string& model_path,
        const std::string& texture_path,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale,
        glm::vec3 color,
        glm::vec3 range,
        usz planet_count_)
    :
    base(create_object(group, model_path, texture_path))
    {
        base.m_translation = position;
        base.m_euler_angles = rotation;
        base.m_scale = scale;
        point_light.color = color;
        point_light.position = position;
        point_light.range = range;
        planet_count = planet_count_;
    }

    star::~star() {}

    stargroup::stargroup()
    :
    m_texture_offset_count(0)
    {
        m_base = std::make_unique<rendergroup>("shaders/stars.vert", "shaders/stars.frag");
    }

    stargroup::~stargroup() {}

    void update(stargroup& context, float delta_time)
    {
        context.m_texture_offset_count += delta_time * 0.000015;
        update_float(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_SCROLL_OFFSET, context.m_texture_offset_count);
    }

    void render(stargroup& context, glm::vec3 camera_xyz)
    {
        bind(*context.m_base->m_program);
        for (const star& cur_object : context.m_stars)
        {
            if (glm::distance(camera_xyz, cur_object.base.m_translation) > ZFAR * 0.65)
                continue;
            bind(*context.m_base->m_textures[cur_object.base.m_texture_index]);
            glm::mat4 transform = get_transform_mat(cur_object.base);
            glm::mat4 inverse_transform = glm::inverse(transform);
            update_uint(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_INDEX, cur_object.base.m_texture_index);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::TRANSFORM, transform);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::INVERSE_TRANSFORM, inverse_transform);
            draw(*context.m_base->m_models[cur_object.base.m_model_index]);
        }
    }

    glm::vec3 light_range_constants(float lightrange)
    {
        return glm::vec3(1.0, 4.5 / lightrange, 75.0 / (lightrange * lightrange));
    }
}

}