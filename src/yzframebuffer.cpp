#include <yzframebuffer.hpp>

namespace yz
{
    framebuffer::framebuffer(i32 width, i32 height)
    :
    m_combine("shaders/combine.vert", "shaders/combine.frag", YZ_LOAD_SPIRV),
    m_blur("shaders/blur.vert", "shaders/blur.frag", YZ_LOAD_SPIRV),
    m_width(width),
    m_height(height)
    {
        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, sizeof(SCREEN_VERTICES), &SCREEN_VERTICES, GL_DYNAMIC_STORAGE_BIT);

        glCreateVertexArrays(1, &m_vao);
        glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, (4 * sizeof(float)));
        glEnableVertexArrayAttrib(m_vao, 0);
        glEnableVertexArrayAttrib(m_vao, 1);
        glVertexArrayAttribFormat(m_vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, (2 * sizeof(float)));
        glVertexArrayAttribBinding(m_vao, 0, 0);
        glVertexArrayAttribBinding(m_vao, 1, 0);

        glCreateFramebuffers(1, &m_fbo);
        glCreateTextures(GL_TEXTURE_2D, 2, m_fbtexture);
        for (usz index = 0; index < 2; index++)
        {
            glTextureStorage2D(m_fbtexture[index], 1, GL_RGBA16F, m_width, m_height);
            glTextureParameteri(m_fbtexture[index], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_fbtexture[index], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(m_fbtexture[index], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(m_fbtexture[index], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0 + index, m_fbtexture[index], 0);
        }

        glCreateRenderbuffers(1, &m_rbo);
        glNamedRenderbufferStorage(m_rbo, GL_DEPTH24_STENCIL8, m_width, m_height);
        glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

        GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glNamedFramebufferDrawBuffers(m_fbo, 2, attachments);

        if (glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Framebuffer incomplete\n");

        glCreateFramebuffers(2, m_blurfbo);
        glCreateTextures(GL_TEXTURE_2D, 2, m_blurfb_texture);
        for (usz index = 0; index < 2; index++)
        {
            glTextureStorage2D(m_blurfb_texture[index], 1, GL_RGBA16F, m_width, m_height);
            glTextureParameteri(m_blurfb_texture[index], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_blurfb_texture[index], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(m_blurfb_texture[index], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(m_blurfb_texture[index], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glNamedFramebufferTexture(m_blurfbo[index], GL_COLOR_ATTACHMENT0, m_blurfb_texture[index], 0);

            if (glCheckNamedFramebufferStatus(m_blurfbo[index], GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Framebuffer incomplete\n");
        }

        update_int(m_blur, UNIFORM_LOCATIONS::BLUR_INPUT_IMAGE, 0);
        update_int(m_combine, UNIFORM_LOCATIONS::COMBINE_MAIN_SCENE, 0);
        update_int(m_combine, UNIFORM_LOCATIONS::COMBINE_BLOOM, 1);
    }

    framebuffer::~framebuffer()
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
        glClearNamedFramebufferfv(0, GL_COLOR, 0, CLEAR_COLOR);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &CLEAR_DEPTH);
        glClearNamedFramebufferiv(0, GL_STENCIL, 0, &CLEAR_STENCIL);
        glClearNamedFramebufferfv(framebuffer_.m_fbo, GL_COLOR, 0, CLEAR_COLOR);
        glClearNamedFramebufferfv(framebuffer_.m_fbo, GL_DEPTH, 0, &CLEAR_DEPTH);
        glClearNamedFramebufferiv(framebuffer_.m_fbo, GL_STENCIL, 0, &CLEAR_STENCIL);
        glClearNamedFramebufferfv(framebuffer_.m_fbo, GL_COLOR, 1, CLEAR_COLOR);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_.m_fbo);
    }

    void end_render(framebuffer& framebuffer_)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bool horizontal = true;
        bool first_iteration = true;
        usz amount = 8;
        bind(framebuffer_.m_blur);
        for (usz count = 0; count < amount; count++)
        {
            update_bool(framebuffer_.m_blur, UNIFORM_LOCATIONS::BLUR_HORIZONTAL, horizontal);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_.m_blurfbo[horizontal]);
            glBindTextureUnit(0, first_iteration ? framebuffer_.m_fbtexture[1] : framebuffer_.m_blurfb_texture[!horizontal]);

            glBindVertexArray(framebuffer_.m_vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bind(framebuffer_.m_combine);
        glBindTextureUnit(0, framebuffer_.m_fbtexture[0]);
        glBindTextureUnit(1, framebuffer_.m_blurfb_texture[!horizontal]);
        glBindVertexArray(framebuffer_.m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}