#include <yzctx.hpp>
#include <yzgen.hpp>

#include <chrono>
#include <thread>

namespace yz
{
    void GLAPIENTRY opengl_debug_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* user_param)
    {
        if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " <<  message << std::endl;

        switch (source)
        {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;

        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
    }

    ctx::ctx(bool opengl_debug, u32 seed)
    :
    m_window(),
    m_control_ctx(m_window.m_window)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD\n");
        }

        GLint major;
        GLint minor;
        GLint ext_count;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_NUM_EXTENSIONS, &ext_count);

        std::cout << "OpenGL VERSION: " << major << '.' << minor << std::endl;
        std::cout << "VENDOR: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "RENDERER: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "EXTENSIONS:" << std::endl;
        for (usz count = 0; count < ext_count; count++)
            std::cout << '\t' << glGetStringi(GL_EXTENSIONS, count) << std::endl;

        if (opengl_debug)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(opengl_debug_callback, 0);
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glStencilFunc(GL_EQUAL, 1, 0xFF);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        window_size dimensions = get_size(m_window);
        m_framebuffer = std::make_unique<framebuffer>(dimensions.width, dimensions.height);
        m_stargroup = std::make_unique<rendergroups::stargroup>();
        m_planetgroup = std::make_unique<rendergroups::planetgroup>();

        m_generation = std::make_unique<gen_context>(
            *m_stargroup,
            *m_planetgroup,
            m_global_ubo.point_lights,
            reinterpret_cast<usz&>(m_global_ubo.point_light_count),
            seed, 16);

        glCreateBuffers(1, &m_ubo);
        glNamedBufferStorage(m_ubo, sizeof(ubo_shared), (void*)&m_global_ubo, GL_DYNAMIC_STORAGE_BIT);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo, 0, sizeof(ubo_shared));
        glNamedBufferSubData(m_ubo, 0, sizeof(ubo_shared), (void*)&m_global_ubo);
    }

    ctx::~ctx()
    {
        glDeleteBuffers(1, &m_ubo);
    }

    void run(ctx& ctx_)
    {
        ctx_.m_global_ubo.model = glm::mat4(1.0);
        std::chrono::system_clock::time_point cur_time = std::chrono::high_resolution_clock::now();
        while (!should_close(ctx_.m_window))
        {
            std::chrono::system_clock::time_point new_time = std::chrono::high_resolution_clock::now();
            float delta_time =
                std::chrono::duration<float, std::chrono::milliseconds::period>(new_time - cur_time).count();
            cur_time = new_time;

            update(ctx_.m_window);
            ctx_.m_global_ubo.view = process_controls(ctx_.m_control_ctx, ctx_.m_window.m_window, delta_time);

            window_size dimensions = get_size(ctx_.m_window);
            ctx_.m_global_ubo.projection =
                glm::perspective<float>(glm::radians(get_fov()), (float)dimensions.width / (float)dimensions.height, 0.1, ZFAR);
            ctx_.m_global_ubo.camera_xyz = ctx_.m_control_ctx.m_camera_xyz;

            if (dimensions.width != ctx_.m_framebuffer->m_width || dimensions.height != ctx_.m_framebuffer->m_height)
            {
                glViewport(0, 0, dimensions.width, dimensions.height);
                u32 previous_count = ctx_.m_framebuffer->m_screen_tearing_count;
                ctx_.m_framebuffer = std::make_unique<framebuffer>(dimensions.width, dimensions.height, previous_count);
            }

            if (!ctx_.m_control_ctx.m_freeze.toggled) update(*ctx_.m_generation, delta_time, *ctx_.m_stargroup, *ctx_.m_planetgroup);
            rendergroups::update(*ctx_.m_stargroup, delta_time);
            rendergroups::update(*ctx_.m_planetgroup, delta_time);

            glNamedBufferSubData(
                ctx_.m_ubo,
                offsetof(ubo_shared, model), offsetof(ubo_shared, point_lights),
                (void*)(&ctx_.m_global_ubo + offsetof(ubo_shared, model)));

            prepare_render(*ctx_.m_framebuffer);
                if (ctx_.m_control_ctx.m_wireframe.toggled) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); };
                rendergroups::render(*ctx_.m_stargroup, ctx_.m_global_ubo.camera_xyz);
                rendergroups::render(*ctx_.m_planetgroup, ctx_.m_global_ubo.camera_xyz);
                if (ctx_.m_control_ctx.m_wireframe.toggled) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); };
            end_render(*ctx_.m_framebuffer, delta_time);
            swap_buffers(ctx_.m_window);
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }
}