#include <yzctx.hpp>
#include <random>

namespace yz
{
    void GLAPIENTRY opengl_debug_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam )
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

    ctx_::ctx_(bool opengl_debug, bool wireframe)
    :
    m_window(framebuffer_size_callback),
    m_control_ctx(m_window.m_window),
    m_wireframe(wireframe)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD\n");
        }

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

        // Generate stars
        {
            m_maingroup = std::make_unique<rendergroups::maingroup>();
            m_no_lightgroup = std::make_unique<rendergroups::no_lightgroup>();

            u32 rand_state = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            std::cout << "SEED: " << rand_state << std::endl;
            for (std::size_t index = 0; index < MAX_POINT_LIGHT; index++)
            {
                glm::vec3 position = {
                    lehmer_randrange_flt(rand_state, -1000.f, 1000.f),
                    lehmer_randrange_flt(rand_state, -1000.f, 1000.f),
                    lehmer_randrange_flt(rand_state, -1000.f, 1000.f)};
                float scale = lehmer_randrange_flt(rand_state, 1.f, 100.f);
                glm::vec3 color = {
                    lehmer_randrange_flt(rand_state, 0.f, 100.f),
                    lehmer_randrange_flt(rand_state, 0.f, 100.f),
                    lehmer_randrange_flt(rand_state, 0.f, 100.f)};

                add_object(*m_maingroup->m_base, "models/uvs2.obj", "textures/unoise.jpg");
                m_maingroup->m_base->m_objects[index].m_translation = position;
                m_maingroup->m_base->m_objects[index].m_scale = glm::vec3(scale, scale, scale);
                rendergroups::add_point_light(*m_maingroup, position, rendergroups::RANGES[9], color);
            }

            //add_object(*m_no_lightgroup->m_base, "models/sphere.obj", "textures/skybox.png");
            //m_no_lightgroup->m_base->m_objects[0].m_scale = glm::vec3(5000.f, 5000.f, 5000.f);
        }
    }

    ctx_::~ctx_() {}

    void run(ctx& ctx_)
    {
        std::chrono::system_clock::time_point cur_time = std::chrono::high_resolution_clock::now();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        while (!should_close(ctx_.m_window))
        {
            std::chrono::system_clock::time_point new_time = std::chrono::high_resolution_clock::now();
            float delta_time =
                std::chrono::duration<float, std::chrono::milliseconds::period>(new_time - cur_time).count();
            cur_time = new_time;

            update(ctx_.m_window);

            view = process_input(ctx_.m_control_ctx, ctx_.m_window.m_window, delta_time);

            prepare_render(*ctx_.m_framebuffer);

            window_size dimensions = get_size(ctx_.m_window);
            projection = glm::perspective(glm::radians(get_fov()), (float)dimensions.width / (float)dimensions.height, 0.1f, 10000.0f);
            if (dimensions.width != ctx_.m_framebuffer->m_width || dimensions.height != ctx_.m_framebuffer->m_height)
                ctx_.m_framebuffer = std::make_unique<framebuffer>(dimensions.width, dimensions.height);
            rendergroups::update(*ctx_.m_maingroup, model, view, projection, ctx_.m_control_ctx.m_camera_xyz, delta_time);
            //rendergroups::update(*ctx_.m_no_lightgroup, model, view, projection);

            if (ctx_.m_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            rendergroups::render(*ctx_.m_maingroup);
            //rendergroups::render(*ctx_.m_no_lightgroup);
            if (ctx_.m_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            end_render(*ctx_.m_framebuffer);

            present(ctx_.m_window);
        }
    }

    i32 lehmer_randrange(u32& state, i32 min, i32 max)
    {
        state = (std::uint64_t)state * 279470273u % 0xfffffffb;
        return (state % (max - min)) + min;
    }

    float lehmer_randrange_flt(u32& state, float min, float max)
    {
        state = (std::uint64_t)state * 279470273u % 0xfffffffb;
        return (float)state / (float)(UINT32_MAX / (max - min)) + min;
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
}