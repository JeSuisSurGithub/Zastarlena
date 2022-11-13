#ifndef YZFRAMEBUFFER_HPP
#define YZFRAMEBUFFER_HPP

#include "yzcommon.hpp"
#include "yzshader.hpp"

namespace yz
{
    constexpr float SCREEN_VERTICES[] =
    {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    constexpr GLfloat CLEAR_COLOR[4] = {0.005f, 0.0f, 0.01f, 1.0f};
    constexpr GLfloat CLEAR_DEPTH = 1.f;
    constexpr GLint CLEAR_STENCIL = 1;

    typedef struct framebuffer
    {
        framebuffer(const framebuffer &) = delete;
        framebuffer &operator=(const framebuffer &) = delete;
        framebuffer(framebuffer &&) = delete;
        framebuffer &operator=(framebuffer &&) = delete;

        shader m_combine;
        shader m_blur;

        GLuint m_vao;
        GLuint m_vbo;

        GLuint m_fbo;
        GLuint m_fbtexture[2];
        GLuint m_rbo;

        GLuint m_blurfbo[2];
        GLuint m_blurfb_texture[2];

        i32 m_width;
        i32 m_height;

        framebuffer(i32 width, i32 height);
        ~framebuffer();
    }framebuffer;

    void prepare_render(framebuffer& framebuffer_);
    void end_render(framebuffer& framebuffer_);
}

#endif /* YZFRAMEBUFFER_HPP */