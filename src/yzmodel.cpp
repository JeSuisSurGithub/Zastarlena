#include <yzmodel.hpp>

#include <algorithm>

#include <ext/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <ext/tiny_obj_loader.h>

namespace yz
{
    model_::model_(
        const std::string& model_path,
        glm::vec3 default_color)
    :
    m_model_path(model_path),
    m_height_map_path("")
    {
        load_obj(default_color);
        load_obj_to_gpu();
    }
    model_::model_(
            const std::string& model_path,
            const std::string& height_map_path,
            glm::vec3 default_color
        )
    :
    m_model_path(model_path),
    m_height_map_path(height_map_path)
    {
        load_obj(default_color);
        int width;
        int height;
        int channels;
        stbi_uc* data = stbi_load(m_height_map_path.c_str(), &width, &height, &channels, 0);
        if (!data)
            throw std::invalid_argument("Could not load height map\n");

        for (std::size_t index = 0; index < m_vertices.size(); index++)
        {
            int pixel_index = std::min(((((m_vertices[index].uv.t) * width) * width) + ((1 - m_vertices[index].uv.s) * height)) * channels, ((float)width * (float)height * channels) - 1);
            float displacement = (255 - data[pixel_index]) / 255.0 + 0.5;
            displacement = std::clamp(displacement, 1.1f, 1.2f);
            m_vertices[index].xyz += glm::normalize(m_vertices[index].xyz) * displacement;
            m_vertices[index].normal += glm::normalize(m_vertices[index].normal) * displacement;
        }
        stbi_image_free(data);
        load_obj_to_gpu();
    }

    void model_::load_obj(glm::vec3 default_color)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, m_model_path.c_str()))
            throw std::runtime_error(warn + err);

        std::unordered_map<vertex, uint32_t> unique_vertices{};

        for (const tinyobj::shape_t& shape : shapes)
        {
            for (const tinyobj::index_t& index : shape.mesh.indices)
            {
                vertex vertex{};
                vertex.xyz =
                {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.uv =
                {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.normal =
                {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };

                vertex.rgb = default_color;

                if (unique_vertices.count(vertex) == 0)
                {
                    unique_vertices[vertex] = static_cast<uint32_t>(m_vertices.size());
                    m_vertices.push_back(vertex);
                }

                m_indices.push_back(unique_vertices[vertex]);
            }
        }
    }

    void model_::load_obj_to_gpu()
    {
        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, m_vertices.size() * sizeof(vertex), m_vertices.data(), GL_DYNAMIC_STORAGE_BIT);

        glCreateBuffers(1, &m_ebo);
        glNamedBufferStorage(m_ebo, m_indices.size() * sizeof(u32), m_indices.data(), GL_DYNAMIC_STORAGE_BIT);

        glCreateVertexArrays(1, &m_vao);

        glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(vertex));
        glVertexArrayElementBuffer(m_vao, m_ebo);

        glEnableVertexArrayAttrib(m_vao, 0);
        glEnableVertexArrayAttrib(m_vao, 1);
        glEnableVertexArrayAttrib(m_vao, 2);
        glEnableVertexArrayAttrib(m_vao, 3);

        glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, xyz));
        glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, rgb));
        glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(vertex, uv));
        glVertexArrayAttribFormat(m_vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, normal));

        glVertexArrayAttribBinding(m_vao, 0, 0);
        glVertexArrayAttribBinding(m_vao, 1, 0);
        glVertexArrayAttribBinding(m_vao, 2, 0);
        glVertexArrayAttribBinding(m_vao, 3, 0);
    }

    model_::~model_()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }

    void draw(const model& model_)
    {
        glBindVertexArray(model_.m_vao);
        glDrawElements(GL_TRIANGLES, model_.m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}