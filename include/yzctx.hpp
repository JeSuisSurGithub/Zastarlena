#ifndef YZCTX_HPP
#define YZCTX_HPP

#include "yzcommon.hpp"
#include "yzcontrol.hpp"
#include "rendergroups/planetgroup.hpp"
#include "rendergroups/stargroup.hpp"
#include "yzframebuffer.hpp"
#include "yzgen.hpp"
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

        bool m_wireframe;
        window m_window;
        controls m_control_ctx;

        std::unique_ptr<framebuffer> m_framebuffer;
        std::unique_ptr<rendergroups::stargroup> m_stargroup;
        std::unique_ptr<rendergroups::planetgroup> m_planetgroup;
        std::unique_ptr<gen_context> m_generation;
        ubo_shared m_global_ubo;
        GLuint m_ubo;

        ctx(bool opengl_debug, bool wireframe);
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
    void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
}

#endif /* YZCTX_HPP */