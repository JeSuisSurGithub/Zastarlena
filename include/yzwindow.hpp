#ifndef YZWINDOW_HPP
#define YZWINDOW_HPP

#include "yzcommon.hpp"

namespace yz
{
    namespace window
    {
        typedef glm::ivec2 window_size;
        typedef glm::vec2 cursor_pos;

        typedef struct window
        {
            window(const window &) = delete;
            window &operator=(const window &) = delete;
            window(window &&) = delete;
            window &operator=(window &&) = delete;

            GLFWwindow* m_window;

            window();
            ~window();
        }window;

        void update(window& window_);
        void swap_buffers(window& window_);
        bool is_focused(const window& window_);
        bool should_close(const window& window_);
        window_size get_size(const window& window_);

        void set_scroll_callback(window& window_, GLFWscrollfun callback);
        void set_cursor_state(window& window_, bool show);
        void set_cursor_pos(window& window_, cursor_pos cursor_pos_);
        cursor_pos get_cursor_pos(window& window_);
        bool is_pressed(const window& window_, u32 glfw_key);
    }
}

#endif /* YZWINDOW_HPP */