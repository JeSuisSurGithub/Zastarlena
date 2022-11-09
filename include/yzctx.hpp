#ifndef YZCTX_HPP
#define YZCTX_HPP

#include "yzcommon.hpp"
#include "yzcontrol.hpp"
#include "rendergroups/maingroup.hpp"
#include "rendergroups/no_light.hpp"
#include "yzframebuffer.hpp"
#include "yzwindow.hpp"

#include <chrono>

namespace yz
{
    typedef struct ctx_
    {
        ctx_(const ctx_ &) = delete;
        ctx_ &operator=(const ctx_ &) = delete;
        ctx_(ctx_ &&) = delete;
        ctx_ &operator=(ctx_ &&) = delete;

        bool m_wireframe;
        window m_window;
        controls m_control_ctx;

        std::unique_ptr<framebuffer> m_framebuffer;
        std::unique_ptr<rendergroups::maingroup> m_maingroup;
        std::unique_ptr<rendergroups::no_lightgroup> m_no_lightgroup;

        ubo_shared m_global_ubo;
        GLuint m_ubo;

        ctx_(bool opengl_debug, bool wireframe);
        ~ctx_();
    }ctx;

    void GLAPIENTRY opengl_debug_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);

    void run(ctx& ctx_);

    i32 lehmer_randrange(u32& state, i32 min, i32 max);
    float lehmer_randrange_flt(u32& state, float min, float max);

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}

#endif /* YZCTX_HPP */