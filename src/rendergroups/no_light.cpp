#include <rendergroups/no_light.hpp>

namespace yz
{

namespace rendergroups
{
    no_lightgroup_::no_lightgroup_()
    {
        m_base = std::make_unique<rendergroup>("shaders/no_lighting.vert.spv", "shaders/no_lighting.frag.spv");
    }

    no_lightgroup_::~no_lightgroup_() {}

    void update(no_lightgroup& context, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::MODEL, glm::value_ptr(model));
        send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::VIEW, glm::value_ptr(view));
        send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::PROJECTION, glm::value_ptr(projection));
    }

    void render(no_lightgroup& context)
    {
        context.m_base->m_program->activate();
        for (const object& cur_object : context.m_base->m_objects)
        {
            context.m_base->m_textures[cur_object.m_texture_index]->activate();
            send_int(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_INDEX, cur_object.m_texture_index);
            glm::mat4 transform = get_transform_mat(cur_object);
            send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::TRANSFORM, glm::value_ptr(transform));
            draw(*context.m_base->m_models[cur_object.m_model_index]);
        }
    }
}

}
