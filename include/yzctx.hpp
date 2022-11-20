#ifndef YZCTX_HPP
#define YZCTX_HPP

#include "yzcommon.hpp"
#include "yzcontrol.hpp"
#include "rendergroups/planetgroup.hpp"
#include "rendergroups/stargroup.hpp"
#include "rendergroups/textgroup.hpp"
#include "yzframebuffer.hpp"
#include "yzgen.hpp"
#include "yzubo.hpp"
#include "yzwindow.hpp"

#include <chrono>
#include <memory>

namespace yz
{
    typedef struct ctx
    {
        ctx(const ctx &) = delete;
        ctx &operator=(const ctx &) = delete;
        ctx(ctx &&) = delete;
        ctx &operator=(ctx &&) = delete;

        window::window m_window;
        controls::controls m_controls;

        std::unique_ptr<framebuffer::framebuffer> m_framebuffer;
        std::unique_ptr<rendergroups::stargroup> m_stargroup;
        std::unique_ptr<rendergroups::planetgroup> m_planetgroup;
        std::unique_ptr<rendergroups::textgroup> m_textgroup;
        std::unique_ptr<memory::ubo> m_ubo;

        ctx(bool opengl_debug, u32 seed);
        ~ctx();
    }ctx;

    void GLAPIENTRY opengl_debug_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* user_param);

    void run(ctx& ctx_);
}

#endif /* YZCTX_HPP */