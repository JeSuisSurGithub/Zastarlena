#include <yztexture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb_image.h>

namespace yz
{
namespace texture
{
    texture::texture(u32 width, u32 height, u32 index, GLenum S_WRAP, GLenum T_WRAP)
    :
    m_index(index),
    m_width(width),
    m_height(height)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, S_WRAP);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, T_WRAP);
        glTextureStorage2D(m_id, 1, GL_RGBA16F, m_width, m_height);
    }

    texture::texture(const std::string& texture_path, u32 index, GLenum S_WRAP, GLenum T_WRAP)
    :
    m_texture_path(texture_path),
    m_index(index)
    {
        int width;
        int height;
        int channels;
        stbi_uc* data = stbi_load(m_texture_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data) { throw std::invalid_argument("Could not load texture\n"); }

        m_width = width;
        m_height = height;
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, S_WRAP);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, T_WRAP);
        glTextureParameteri(m_id, GL_TEXTURE_MAX_ANISOTROPY, 2.0f);
        glTextureStorage2D(m_id, 1, GL_RGBA8, m_width, m_height);
        glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

    void bind(texture& texture_, u32 index)
    {
        glBindTextureUnit(index, texture_.m_id);
    }

    void bind_to_framebuffer(texture& texture_, GLuint framebuffer, u32 index)
    {
        glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0 + index, texture_.m_id, 0);
    }
}
}