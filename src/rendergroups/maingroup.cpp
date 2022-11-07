#include <rendergroups/maingroup.hpp>

namespace yz
{

namespace rendergroups
{
    maingroup_::maingroup_()
    :
    m_current_point_light_count(0),
    m_texture_offset_count(0)
    {
        m_base = std::make_unique<rendergroup>("shaders/main.vert.spv", "shaders/main.frag.spv");
    }

    maingroup_::~maingroup_() {}

    void update(maingroup& context, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camera_xyz, float delta_time)
    {
        for (size_t index = 0; index < context.m_current_point_light_count; index++)
        {
            send_vec3(*context.m_base->m_program, UNIFORM_LOCATIONS::POINT_LIGHTS_POSITION + index, context.m_point_light_position[index]);
            send_vec3(*context.m_base->m_program, UNIFORM_LOCATIONS::POINT_LIGHTS_ATTENUATION_CONST + index, context.m_point_light_range[index]);
            send_vec3(*context.m_base->m_program, UNIFORM_LOCATIONS::POINT_LIGHTS_COLOR + index, context.m_point_light_color[index]);
        }

        send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::MODEL, glm::value_ptr(model));
        send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::VIEW, glm::value_ptr(view));
        send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::PROJECTION, glm::value_ptr(projection));
        //send_vec3(*context.m_base->m_program, UNIFORM_LOCATIONS::CAMERA_XYZ, camera_xyz);
        context.m_texture_offset_count += delta_time * 0.000015f;
        send_float(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_SCROLL_OFFSET, context.m_texture_offset_count);
    }

    void render(maingroup& context)
    {
        context.m_base->m_program->activate();
        for (const object& cur_object : context.m_base->m_objects)
        {
            context.m_base->m_textures[cur_object.m_texture_index]->activate();
            send_int(*context.m_base->m_program, UNIFORM_LOCATIONS::TEXTURE_INDEX, cur_object.m_texture_index);
            glm::mat4 transform = get_transform_mat(cur_object);
            glm::mat4 inverse_transform = glm::inverse(transform);
            send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::TRANSFORM, glm::value_ptr(transform));
            send_matrix4(*context.m_base->m_program, UNIFORM_LOCATIONS::INVERSE_TRANSFORM, glm::value_ptr(inverse_transform));
            draw(*context.m_base->m_models[cur_object.m_model_index]);
        }
    }

    void add_point_light(maingroup& context, glm::vec3 position, glm::vec3 range, glm::vec3 color)
    {
        context.m_point_light_position[context.m_current_point_light_count] = position;
        context.m_point_light_range[context.m_current_point_light_count] = range;
        context.m_point_light_color[context.m_current_point_light_count] = color;
        context.m_current_point_light_count++;
        send_int(*context.m_base->m_program, UNIFORM_LOCATIONS::CURRENT_POINT_LIGHT_COUNT, context.m_current_point_light_count);
    }
}

}