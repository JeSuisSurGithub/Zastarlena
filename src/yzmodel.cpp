#include <yzmodel.hpp>

#include <algorithm>

#include <ext/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <ext/tiny_obj_loader.h>

namespace yz
{
    model::model(
        const std::string& model_path,
        glm::vec3 default_color)
    :
    m_model_path(model_path),
    m_height_map_path(""),
    m_color(default_color)
    {
        load_obj(default_color);
        load_obj_to_gpu();
    }

    model::model(
            const std::string& model_path,
            const std::string& height_map_path,
            glm::vec3 default_color,
            std::function<float(float)> height_filter
        )
    :
    m_model_path(model_path),
    m_height_map_path(height_map_path),
    m_color(default_color)
    {
        load_obj(default_color);
        int width;
        int height;
        int channels;
        stbi_uc* data = stbi_load(m_height_map_path.c_str(), &width, &height, &channels, 0);
        if (!data)
            throw std::invalid_argument("Could not load height map\n");

        for (usz index = 0; index < m_vertices.size(); index++)
        {
            int pixel_index = std::min(((((m_vertices[index].uv.t) * width) * width) + ((1 - m_vertices[index].uv.s) * height)) * channels, ((float)width * (float)height * channels) - 1);
            float pixel_value = data[pixel_index] / 255.0;
            m_vertices[index].xyz += glm::normalize(m_vertices[index].xyz) * height_filter(pixel_value);
        }
        stbi_image_free(data);
        load_obj_to_gpu();
    }

    void model::load_obj(glm::vec3 default_color)
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

    void model::load_obj_to_gpu()
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

    model::~model()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }

    float default_height_filter(float pixel_value)
    {
        float displacement =  (1.f - pixel_value) + 1.0;
        return displacement / 2.f;
        //return std::clamp(displacement, 1.0f, 1.2f);
    }

    void draw(model& model_)
    {
        glBindVertexArray(model_.m_vao);
        glDrawElements(GL_TRIANGLES, model_.m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}