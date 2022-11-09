#include <rendergroups/maingroup.hpp>

namespace yz
{

namespace rendergroups
{
    maingroup_::maingroup_()
    :
    m_texture_offset_count(0)
    {
        m_base = std::make_unique<rendergroup>("shaders/main.vert", "shaders/main.frag");
    }

    maingroup_::~maingroup_() {}

    void update(maingroup& context, float delta_time)
    {
        context.m_texture_offset_count += delta_time * 0.000015f;
        update_float(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_SCROLL_OFFSET, context.m_texture_offset_count);
    }

    void render(maingroup& context)
    {
        bind(*context.m_base->m_program);
        for (const object& cur_object : context.m_base->m_objects)
        {
            bind(*context.m_base->m_textures[cur_object.m_texture_index]);
            glm::mat4 transform = get_transform_mat(cur_object);
            glm::mat4 inverse_transform = glm::inverse(transform);
            //send_int(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_INDEX, cur_object.m_texture_index);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::TRANSFORM, transform);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::INVERSE_TRANSFORM, inverse_transform);
            draw(*context.m_base->m_models[cur_object.m_model_index]);
        }
    }
}

}