#include <yzrendergroup.hpp>

#include <glm/gtx/quaternion.hpp>

namespace yz
{
    object::object(u32 model_index, u32 texture_index)
    :
    m_model_index(model_index),
    m_texture_index(texture_index),
    m_translation({0.f, 0.f, 0.f}),
    m_euler_angles({0.f, 0.f, 0.f}),
    m_scale({1.f, 1.f, 1.f})
    {
    }

    object::~object() {}

    void translate(object& object_, glm::vec3 translation) { object_.m_translation += translation; }
    void rotate(object& object_, glm::vec3 euler_angles) { object_.m_euler_angles += euler_angles; }
    void scale(object& object_, glm::vec3 scale) { object_.m_scale *= scale; }

    glm::mat4 get_transform_mat(const object& object_)
    {
        glm::mat4 translation_mat = glm::translate(glm::mat4(1.f), object_.m_translation);
        glm::mat4 rotation_mat = glm::toMat4(glm::quat(object_.m_euler_angles));
        glm::mat4 scale_mat = glm::scale(glm::mat4(1.f), object_.m_scale);
        return translation_mat * rotation_mat * scale_mat;
    }

    rendergroup::rendergroup(const std::string& vert_path, const std::string& frag_path)
    :
    m_program(std::make_unique<shader>(vert_path, frag_path, YZ_LOAD_SPIRV)),
    m_textures(),
    m_models(),
    m_objects()
    {
        GLint values[MAX_TEXTURE_COUNT];
        for (usz index = 0; index < MAX_TEXTURE_COUNT; index++)
            values[index] = index;
        update_int_array(*m_program, UNIFORM_LOCATIONS::TEXTURE, values, MAX_TEXTURE_COUNT);
    }

    rendergroup::~rendergroup() {}

    void add_object(rendergroup& group, const std::string& model_path, const std::string& texture_path)
    {
        u32 model_index = add_model(group, model_path);
        u32 texture_index = add_texture(group, texture_path);
        add_object(group, model_index, texture_index);
    }

    void add_object(
        rendergroup& group,
        const std::string& model_path,
        const std::string& texture_path,
        const std::string& height_map_path,
        glm::vec3 default_color)
    {
        u32 model_index = add_model(group, model_path, height_map_path, default_color);
        u32 texture_index = add_texture(group, texture_path);

        add_object(group, model_index, texture_index);
    }

    void add_object(rendergroup& group, u32 model_index, u32 texture_index)
    {
        group.m_objects.push_back(object{model_index, texture_index});
    }

    u32 add_model(rendergroup& group, const std::string& model_path)
    {
        std::vector<std::unique_ptr<model>>::iterator iterator = std::find_if(
            group.m_models.begin(),
            group.m_models.end(),
            [&](std::unique_ptr<model>& model) -> bool
        {
            return (model->m_model_path == model_path);
        });

        u32 model_index{0};
        if (iterator == std::end(group.m_models))
        {
            model_index = group.m_models.size();
            group.m_models.push_back(std::make_unique<model>(model_path));
        }
        else
        {
            model_index = std::distance(std::begin(group.m_models), iterator);
        }
        return model_index;
    }

    u32 add_model(
        rendergroup& group,
        const std::string& model_path,
        const std::string& height_map_path,
        glm::vec3 default_color)
    {
        std::vector<std::unique_ptr<model>>::iterator iterator = std::find_if(
            group.m_models.begin(),
            group.m_models.end(),
            [&](std::unique_ptr<model>& model) -> bool
        {
            return (model->m_model_path == model_path) &&
                (model->m_height_map_path == height_map_path) &&
                (model->m_color == default_color);
        });

        u32 model_index{0};
        if (iterator == std::end(group.m_models))
        {
            model_index = group.m_models.size();
            group.m_models.push_back(std::make_unique<model>(model_path, height_map_path, default_color));
        }
        else
        {
            model_index = std::distance(std::begin(group.m_models), iterator);
        }
        return model_index;
    }

    u32 add_texture(rendergroup& group, const std::string& texture_path)
    {
        if (group.m_textures.size() == MAX_TEXTURE_COUNT)
            throw std::logic_error("Maximum texture count reached");

        std::vector<std::unique_ptr<texture>>::iterator iterator = std::find_if(
            group.m_textures.begin(),
            group.m_textures.end(),
            [&](std::unique_ptr<texture>& texture) -> bool
        {
            return (texture->m_texture_path == texture_path);
        });

        u32 texture_index{0};
        if (iterator == std::end(group.m_textures))
        {
            texture_index = group.m_textures.size();
            group.m_textures.push_back(std::make_unique<texture>(texture_path, texture_index));
        }
        else
        {
            texture_index = std::distance(std::begin(group.m_textures), iterator);
        }
        return texture_index;
    }
}