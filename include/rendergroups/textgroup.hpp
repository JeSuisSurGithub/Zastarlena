#ifndef TEXTGROUP_HPP
#define TEXTGROUP_HPP

#include "yzcommon.hpp"
#include "yzshader.hpp"
#include "yztexture.hpp"

namespace yz
{
    namespace rendergroups
    {
        typedef struct textgroup
        {
            textgroup(const textgroup &) = delete;
            textgroup &operator=(const textgroup &) = delete;
            textgroup(textgroup &&) = delete;
            textgroup &operator=(textgroup &&) = delete;

            const usz m_size;
            std::string m_text;
            std::vector<glm::vec4> m_vertices;

            GLuint m_vao;
            GLuint m_vbo;

            texture::texture m_characters;
            shader::shader m_program;

            textgroup(const std::string& text, usz size);
            ~textgroup();
        }textgroup;

        void render(textgroup& group, glm::vec2 framebuffer_size);
    }
}

#endif /* TEXTGROUP_HPP */