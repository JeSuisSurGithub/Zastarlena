#ifndef YZRENDERGROUPS_HPP
#define YZRENDERGROUPS_HPP

#include "yzshader.hpp"
#include "yztexture.hpp"
#include "yzmodel.hpp"

#include <memory>

namespace yz
{
    typedef struct object
    {
        u32 m_model_index;
        u32 m_texture_index;

        glm::vec3 m_translation;
        glm::vec3 m_euler_angles;
        glm::vec3 m_scale;

        object(u32 model_index, u32 texture_index);
        ~object();
    }object;

    void translate(object& object_, glm::vec3 translation);
    void rotate(object& object_, glm::vec3 euler_angles);
    void scale(object& object_, glm::vec3 scale);
    glm::mat4 get_transform_mat(const object& object_);

    typedef struct rendergroup
    {
        rendergroup(const rendergroup &) = delete;
        rendergroup &operator=(const rendergroup &) = delete;
        rendergroup(rendergroup &&) = delete;
        rendergroup &operator=(rendergroup &&) = delete;

        std::unique_ptr<shader> m_program;
        std::vector<std::unique_ptr<texture>> m_textures;
        std::vector<std::unique_ptr<model>> m_models;
        std::vector<object> m_objects;

        rendergroup(const std::string& vert_path, const std::string& frag_path);
        ~rendergroup();
    }rendergroup;

    void add_object(rendergroup& group, const std::string& model_path, const std::string& texture_path);
    void add_object(
        rendergroup& group,
        const std::string& model_path,
        const std::string& texture_path,
        const std::string& height_map_path,
        glm::vec3 default_color);
    void add_object(rendergroup& group, u32 model_index, u32 texture_index);

    u32 add_model(rendergroup& group, const std::string& model_path);
    u32 add_model(
        rendergroup& group,
        const std::string& model_path,
        const std::string& height_map_path,
        glm::vec3 default_color);
    u32 add_texture(rendergroup& group, const std::string& texture_path);
}

#endif /* YZRENDERGROUPS_HPP */