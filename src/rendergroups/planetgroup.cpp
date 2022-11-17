#include <rendergroups/planetgroup.hpp>

namespace yz
{

namespace rendergroups
{
    planetgroup::planetgroup()
    {
        m_base = std::make_unique<rendergroup>("shaders/planets.vert", "shaders/planets.frag");
    }

    planetgroup::~planetgroup() {}

    void update(planetgroup& context, float delta_time)
    {
        return;
    }

    void render(planetgroup& context, glm::vec3 camera_xyz)
    {
        bind(*context.m_base->m_program);
        for (const object& cur_object : context.m_base->m_objects)
        {
            if (glm::distance(camera_xyz, cur_object.m_translation) > ZFAR * 0.65)
                continue;
            bind(*context.m_base->m_textures[cur_object.m_texture_index]);
            glm::mat4 transform = get_transform_mat(cur_object);
            glm::mat4 inverse_transform = glm::inverse(transform);
            update_uint(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_INDEX, cur_object.m_texture_index);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::TRANSFORM, transform);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::INVERSE_TRANSFORM, inverse_transform);
            draw(*context.m_base->m_models[cur_object.m_model_index]);
        }
    }
}

}
