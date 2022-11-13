#include <yztexture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb_image.h>

namespace yz
{
    texture::texture(const std::string& texture_path, u32 index)
    :
    m_index(index),
    m_texture_path(texture_path)
    {
        int width;
        int height;
        int channels;
        stbi_uc* data = stbi_load(m_texture_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data)
            throw std::invalid_argument("Could not load texture\n");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage2D(m_id, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_id);
        stbi_image_free(data);
    }

    texture::~texture()
    {
        glDeleteTextures(1, &m_id);
    }

    void bind(texture& texture_)
    {
        glBindTextureUnit(texture_.m_index, texture_.m_id);
    }
}