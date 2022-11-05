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

    void update(window&);

    void present(window&);

    window_size get_size(const window&);

    bool should_close(const window&);
}

#endif /* YZWINDOW_HPP */