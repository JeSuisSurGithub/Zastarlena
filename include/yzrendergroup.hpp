#ifndef YZRENDERGROUPS_HPP
#define YZRENDERGROUPS_HPP

#include "yzcommon.hpp"
#include "yzshader.hpp"
#include "yztexture.hpp"
#include "yzmodel.hpp"

#include <memory>

namespace yz
{
    typedef struct object_
    {
        u32 m_model_index;
        u32 m_texture_index;

        glm::vec3 m_translation;
        glm::vec3 m_euler_angles;
        glm::vec3 m_scale;

        object_(u32 model_index, u32 texture_index);
        ~object_();
    }object;

    void translate(object& object_, glm::vec3 translation);
    void rotate(object& object_, glm::vec3 euler_angles);
    void scale(object& object_, glm::vec3 scale);
    glm::mat4 get_transform_mat(const object& object);

    typedef struct rendergroup_
    {
        rendergroup_(const rendergroup_ &) = delete;
        rendergroup_ &operator=(const rendergroup_ &) = delete;
        rendergroup_(rendergroup_ &&) = delete;
        rendergroup_ &operator=(rendergroup_ &&) = delete;

        std::unique_ptr<shader> m_program;
        std::vector<std::unique_ptr<texture>> m_textures;
        std::vector<std::unique_ptr<model>> m_models;
        std::vector<object> m_objects;

        rendergroup_(const std::string& vert_path, const std::string& frag_path);
        ~rendergroup_();
    }rendergroup;

    void add_object(rendergroup_& group_ctx, const std::string& model_path, const std::string& texture_path);
    void add_object(rendergroup_& group_ctx, u32 model_index, u32 texture_index);

    u32 add_model(rendergroup_& group_ctx, const std::string& model_path);
    u32 add_texture(rendergroup_& group_ctx, const std::string& texture_path);
}

#endif /* YZRENDERGROUPS_HPP */