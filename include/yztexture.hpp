#ifndef YZTEXTURE_HPP
#define YZTEXTURE_HPP

#include "yzcommon.hpp"

namespace yz
{
    typedef struct texture_
    {
        texture_(const texture_ &) = delete;
        texture_ &operator=(const texture_ &) = delete;
        texture_(texture_ &&) = delete;
        texture_ &operator=(texture_ &&) = delete;

        GLuint m_id;
        u32 m_index;

        const std::string m_texture_path;

        texture_(const std::string& texture_path, u32 index);
        ~texture_();
    }texture;

    void bind(texture& texture_);
}

#endif /* YZTEXTURE_HPP */