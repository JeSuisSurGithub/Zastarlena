#include <rendergroups/textgroup.hpp>

namespace yz
{
namespace rendergroups
{
    textgroup::textgroup(const std::string& text, usz size)
    :
    m_size(size),
    m_text(text),
    m_characters("textures/font.png", 0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE),
    m_program("shaders/text.vert", "shaders/text.frag", YZ_LOAD_SPIRV)
    {
        update_int(m_program, UNIFORM_LOCATIONS::CHARACTER_TEXTURE, 0);

        for (usz count = 0; count < m_text.size(); count++)
        {
            glm::vec2 xy_upleft    = glm::vec2(count * m_size     , 0);
            glm::vec2 xy_upright   = glm::vec2(count * m_size + m_size, 0);
            glm::vec2 xy_downright = glm::vec2(count * m_size + m_size, m_size);
            glm::vec2 xy_downleft  = glm::vec2(count * m_size     , m_size);

            char c = m_text[count];
            float uv_x = (c % 16) / 16.0;
            float uv_y = (c / 16) / 16.0;

            glm::vec2 uv_upleft    = glm::vec2(uv_x             , uv_y);
            glm::vec2 uv_upright   = glm::vec2(uv_x + 1.0 / 16.0, uv_y);
            glm::vec2 uv_downright = glm::vec2(uv_x + 1.0 / 16.0, (uv_y + 1.0 / 16.0));
            glm::vec2 uv_downleft  = glm::vec2(uv_x             , (uv_y + 1.0 / 16.0));

            m_vertices.push_back(glm::vec4(xy_upleft, uv_upleft));
            m_vertices.push_back(glm::vec4(xy_downleft, uv_downleft));
            m_vertices.push_back(glm::vec4(xy_upright, uv_upright));

            m_vertices.push_back(glm::vec4(xy_downright, uv_downright));
            m_vertices.push_back(glm::vec4(xy_upright, uv_upright));
            m_vertices.push_back(glm::vec4(xy_downleft, uv_downleft));
        }

        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, m_vertices.size() * sizeof(glm::vec4), m_vertices.data(), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

        glCreateVertexArrays(1, &m_vao);
        glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, (4 * sizeof(float)));
        glEnableVertexArrayAttrib(m_vao, 0);
        glEnableVertexArrayAttrib(m_vao, 1);
        glVertexArrayAttribFormat(m_vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, (2 * sizeof(float)));
        glVertexArrayAttribBinding(m_vao, 0, 0);
        glVertexArrayAttribBinding(m_vao, 1, 0);
    }

    textgroup::~textgroup()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
    }

    void render(textgroup& group, glm::vec2 framebuffer_size)
    {
        bind(group.m_program);
        bind(group.m_characters);
        update_vec2(group.m_program,
            UNIFORM_LOCATIONS::SCREEN_RESOLUTION, framebuffer_size);
        glBindVertexArray(group.m_vao);
        glDrawArrays(GL_TRIANGLES, 0, group.m_vertices.size());
        glBindVertexArray(0);
    }
}
}