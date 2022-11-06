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

    const GLfloat CLEAR_COLOR[] = {0.005f, 0.0f, 0.01f, 1.0f};
    const GLfloat CLEAR_DEPTH = 1.f;
    const GLint CLEAR_STENCIL = 1;

    typedef struct framebuffer_
    {
        framebuffer_(const framebuffer_ &) = delete;
        framebuffer_ &operator=(const framebuffer_ &) = delete;
        framebuffer_(framebuffer_ &&) = delete;
        framebuffer_ &operator=(framebuffer_ &&) = delete;

        shader m_combine;
        shader m_blur;

        GLuint m_vao;
        GLuint m_vbo;

        GLuint m_fbo;
        GLuint m_fbtexture[2];
        GLuint m_rbo;

        GLuint m_blurfbo[2];
        GLuint m_blurfb_texture[2];

        int m_width;
        int m_height;

        framebuffer_(int width, int height);
        ~framebuffer_();
    }framebuffer;

    void prepare_render(framebuffer& framebuffer_);
    void end_render(framebuffer& framebuffer_);
}

#endif /* YZFRAMEBUFFER_HPP */