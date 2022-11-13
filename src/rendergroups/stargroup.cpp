#include <rendergroups/stargroup.hpp>

namespace yz
{

namespace rendergroups
{
    stargroup::stargroup()
    :
    m_texture_offset_count(0)
    {
        m_base = std::make_unique<rendergroup>("shaders/stars.vert" + SPIRV_EXTENSION, "shaders/stars.frag" + SPIRV_EXTENSION);
    }

    stargroup::~stargroup() {}

    void update(stargroup& context, float delta_time)
    {
        context.m_texture_offset_count += delta_time * 0.000015f;
        update_float(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_SCROLL_OFFSET, context.m_texture_offset_count);
    }

    void render(stargroup& context)
    {
        bind(*context.m_base->m_program);
        for (const object& cur_object : context.m_base->m_objects)
        {
            bind(*context.m_base->m_textures[cur_object.m_texture_index]);
            glm::mat4 transform = get_transform_mat(cur_object);
            glm::mat4 inverse_transform = glm::inverse(transform);
            update_uint(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_INDEX, cur_object.m_texture_index);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::TRANSFORM, transform);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::INVERSE_TRANSFORM, inverse_transform);
            draw(*context.m_base->m_models[cur_object.m_model_index]);
        }
    }

    glm::vec3 light_range_constants(float lightrange)
    {
        return glm::vec3(1.f, 4.5f / lightrange, 75.f / (lightrange * lightrange));
    }
}

}