#ifndef YZTEXTURE_HPP
#define YZTEXTURE_HPP

#include "yzcommon.hpp"

namespace yz
{
    typedef struct texture
    {
        texture(const texture &) = delete;
        texture &operator=(const texture &) = delete;
        texture(texture &&) = delete;
        texture &operator=(texture &&) = delete;

        GLuint m_id;
        u32 m_index;
        const std::string m_texture_path;

        texture(const std::string& texture_path, u32 index);
        ~texture();
    }texture;

    void bind(texture& texture_);
}

#endif /* YZTEXTURE_HPP */