#include <yztexture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb_image.h>

namespace yz
{
    texture_::texture_(const std::string& filename, u32 index)
    :
    m_index(index),
    m_texture_path(filename)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width;
        int height;
        int channels;
        stbi_uc* data = stbi_load(m_texture_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data)
            throw std::invalid_argument("Could not load texture\n");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    texture_::~texture_()
    {
        glDeleteTextures(1, &m_id);
    }
}