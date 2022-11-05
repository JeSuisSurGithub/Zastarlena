#ifndef YZMODEL_HPP
#define YZMODEL_HPP

#include "yzcommon.hpp"

#include <unordered_map>

namespace std
{
    template<> struct hash<yz::vertex>
    {
        std::size_t operator()(yz::vertex const& vertex) const
        {
            return
                ((hash<glm::vec3>()(vertex.xyz) ^
                (hash<glm::vec3>()(vertex.rgb) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };
}

namespace yz
{
    typedef struct model_
    {
        model_(const model_ &) = delete;
        model_ &operator=(const model_ &) = delete;
        model_(model_ &&) = delete;
        model_ &operator=(model_ &&) = delete;

        const std::string m_model_path;

        std::vector<vertex> m_vertices;
        std::vector<u32> m_indices;

        GLuint m_vbo;
        GLuint m_vao;
        GLuint m_ebo;

        model_(
            const std::string& model_path,
            glm::vec3 default_color = {1.f, 1.f, 1.f});
        ~model_();
    }model;

    void draw(const model&);
}

#endif /* YZMODEL_HPP */