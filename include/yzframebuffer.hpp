#ifndef YZFRAMEBUFFER_HPP
#define YZFRAMEBUFFER_HPP

#include "yzcommon.hpp"
#include "yzshader.hpp"

namespace yz
{
    constexpr float SCREEN_VERTICES[] =
    {
        -1.0,  1.0,  0.0, 1.0,
        -1.0, -1.0,  0.0, 0.0,
         1.0, -1.0,  1.0, 0.0,

        -1.0,  1.0,  0.0, 1.0,
         1.0, -1.0,  1.0, 0.0,
         1.0,  1.0,  1.0, 1.0
    };

    constexpr GLfloat CLEAR_COLOR[4] = {0.005, 0.0, 0.01, 1.0};
    constexpr GLfloat CLEAR_DEPTH = 1.0;
    constexpr GLint CLEAR_STENCIL = 1;
    constexpr usz BLOOM_LEVEL = 2;

    typedef struct framebuffer
    {
        framebuffer(const framebuffer &) = delete;
        framebuffer &operator=(const framebuffer &) = delete;
        framebuffer(framebuffer &&) = delete;
        framebuffer &operator=(framebuffer &&) = delete;

        shader m_final;
        shader m_upsampler;
        shader m_downsampler;

        GLuint m_vao;
        GLuint m_vbo;

        GLuint m_fbo;
        GLuint m_fbtexture[2];
        GLuint m_rbo;

        GLuint m_bloom_fbo;
        GLuint m_bloom_fbtextures[BLOOM_LEVEL];
        glm::vec2 m_bloom_levels[BLOOM_LEVEL];

        i32 m_width;
        i32 m_height;

        u32 m_screen_tearing_count;

        framebuffer(i32 width, i32 height, u32 previous_count = 0);
        ~framebuffer();
    }framebuffer;

    void prepare_render(framebuffer& framebuffer_);
    void end_render(framebuffer& framebuffer_, float delta_time);
}

#endif /* YZFRAMEBUFFER_HPP */