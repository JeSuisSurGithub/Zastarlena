#include "yzcommon.hpp"
#include "yzshader.hpp"
#include <yzframebuffer.hpp>

namespace yz
{
    framebuffer_::framebuffer_(int width, int height)
    :
    m_combine("shaders/combine.vert.spv", "shaders/combine.frag.spv"),
    m_blur("shaders/blur.vert.spv", "shaders/blur.frag.spv"),
    m_width(width),
    m_height(height)
    {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_VERTICES), &SCREEN_VERTICES, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        glGenTextures(2, m_fbtexture);
        for (std::size_t index = 0; index < 2; index++)
        {
            glBindTexture(GL_TEXTURE_2D, m_fbtexture[index]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_fbtexture[index], 0);
        }

        glGenRenderbuffers(1, &m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

        GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Framebuffer incomplete\n");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(2, m_blurfbo);
        glGenTextures(2, m_blurfb_texture);
        for (std::size_t index = 0; index < 2; index++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_blurfbo[index]);
            glBindTexture(GL_TEXTURE_2D, m_blurfb_texture[index]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurfb_texture[index], 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Framebuffer not complete!" << std::endl;
        }

        send_int(m_blur, UNIFORM_LOCATIONS::BLUR_INPUT_IMAGE, 0);
        send_int(m_combine, UNIFORM_LOCATIONS::COMBINE_MAIN_SCENE, 0);
        send_int(m_combine, UNIFORM_LOCATIONS::COMBINE_BLURRED, 1);
    }

    framebuffer_::~framebuffer_()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteFramebuffers(1, &m_fbo);
        glDeleteTextures(2, m_fbtexture);
        glDeleteRenderbuffers(1, &m_rbo);
        glDeleteFramebuffers(2, m_blurfbo);
        glDeleteTextures(2, m_blurfb_texture);
    }

    void prepare_render(framebuffer& framebuffer_)
    {
        glClearColor(0.005f, 0.0f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_.m_fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void end_render(framebuffer& framebuffer_)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bool horizontal = true;
        bool first_iteration = true;
        std::size_t amount = 10;
        framebuffer_.m_blur.activate();
        for (std::size_t count = 0; count < amount; count++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_.m_blurfbo[horizontal]);
            send_int(framebuffer_.m_blur, UNIFORM_LOCATIONS::BLUR_HORIZONTAL, horizontal);
            glBindTexture(GL_TEXTURE_2D,
                first_iteration ? framebuffer_.m_fbtexture[1] : framebuffer_.m_blurfb_texture[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
            glBindVertexArray(framebuffer_.m_vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        framebuffer_.m_combine.activate();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, framebuffer_.m_fbtexture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, framebuffer_.m_blurfb_texture[!horizontal]);
        glBindVertexArray(framebuffer_.m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}