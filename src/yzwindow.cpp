#include <yzwindow.hpp>

namespace yz
{
    window_::window_(GLFWframebuffersizefun framebuffer_size_callback)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_window = glfwCreateWindow(1280, 720, WINDOW_NAME.c_str(), NULL, NULL);
        if (!m_window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window\n");
        }

        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    window_::~window_()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void update(window& window_)
    {
        glfwPollEvents();
        if (glfwGetKey(window_.m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window_.m_window, true);
    }

    void present(window& window_)
    {
        glfwSwapBuffers(window_.m_window);
    }

    window_size get_size(const window& window_)
    {
        int width;
        int height;
        glfwGetWindowSize(window_.m_window, &width, &height);
        return {width, height};
    }

    bool should_close(const window& window_)
    {
        return glfwWindowShouldClose(window_.m_window);
    }
}