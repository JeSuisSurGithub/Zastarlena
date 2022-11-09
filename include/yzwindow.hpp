#ifndef YZWINDOW_HPP
#define YZWINDOW_HPP

#include "yzcommon.hpp"

namespace yz
{
    typedef struct window_
    {
        window_(const window_ &) = delete;
        window_ &operator=(const window_ &) = delete;
        window_(window_ &&) = delete;
        window_ &operator=(window_ &&) = delete;

        GLFWwindow* m_window;

        window_(GLFWframebuffersizefun);
        ~window_();
    }window;

    typedef struct window_size_
    {
        int width;
        int height;
    }window_size;

    void update(window& window_);
    void swap_buffers(window& window_);
    window_size get_size(const window& window_);
    bool should_close(const window& window_);
}

#endif /* YZWINDOW_HPP */