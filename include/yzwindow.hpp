#ifndef YZWINDOW_HPP
#define YZWINDOW_HPP

#include "yzcommon.hpp"

namespace yz
{
    typedef struct window
    {
        window(const window &) = delete;
        window &operator=(const window &) = delete;
        window(window &&) = delete;
        window &operator=(window &&) = delete;

        GLFWwindow* m_window;

        window(GLFWframebuffersizefun resize_callback);
        ~window();
    }window;

    typedef struct window_size
    {
        int width;
        int height;
    }window_size;

    void update(window& window);
    void swap_buffers(window& window);
    window_size get_size(const window& window);
    bool should_close(const window& window);
}

#endif /* YZWINDOW_HPP */