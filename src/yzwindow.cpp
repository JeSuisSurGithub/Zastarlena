#include <yzwindow.hpp>

namespace yz
{
    window::window()
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
        glfwSwapInterval(0);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    window::~window()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void update(window& window)
    {
        glfwPollEvents();
        if (glfwGetKey(window.m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window.m_window, true);
    }

    void swap_buffers(window& window)
    {
        glfwSwapBuffers(window.m_window);
    }

    window_size get_size(const window& window)
    {
        int width;
        int height;
        glfwGetWindowSize(window.m_window, &width, &height);
        return {width, height};
    }

    bool should_close(const window& window)
    {
        return glfwWindowShouldClose(window.m_window);
    }
}