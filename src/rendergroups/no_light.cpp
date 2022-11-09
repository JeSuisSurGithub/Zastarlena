#include <rendergroups/no_light.hpp>

namespace yz
{

namespace rendergroups
{
    no_lightgroup_::no_lightgroup_()
    {
        m_base = std::make_unique<rendergroup>("shaders/no_lighting.vert", "shaders/no_lighting.frag");
    }

    no_lightgroup_::~no_lightgroup_() {}

    void update(no_lightgroup& context)
    {
        return;
    }

    void render(no_lightgroup& context)
    {
        bind(*context.m_base->m_program);
        for (const object& cur_object : context.m_base->m_objects)
        {
            bind(*context.m_base->m_textures[cur_object.m_texture_index]);
            glm::mat4 transform = get_transform_mat(cur_object);
            //update_int(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_INDEX, cur_object.m_texture_index);
            update_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::TRANSFORM, transform);
            draw(*context.m_base->m_models[cur_object.m_model_index]);
        }
    }
}

}
