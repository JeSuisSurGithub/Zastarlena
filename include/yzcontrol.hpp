#ifndef YZCONTROL_HPP
#define YZCONTROL_HPP

#include "yzcommon.hpp"

namespace yz
{
    enum KEYMAP
    {
        FORWARD     = GLFW_KEY_W,
        LEFT        = GLFW_KEY_A,
        BACKWARD    = GLFW_KEY_S,
        RIGHT       = GLFW_KEY_D,
        EXIT        = GLFW_KEY_ESCAPE,
        SHOW_MOUSE  = GLFW_KEY_LEFT_ALT
    };

    typedef struct controls_
    {
        controls_(const controls_ &) = delete;
        controls_ &operator=(const controls_ &) = delete;
        controls_(controls_ &&) = delete;
        controls_ &operator=(controls_ &&) = delete;

        bool m_show_mouse;
        float m_move_speed;
        float m_view_speed;
        float m_horizontal_angle;
        float m_vertical_angle;
        u32 m_show_mouse_cooldown;
        u32 m_show_mouse_cooldown_max;
        glm::vec3 m_camera_xyz;

        controls_(GLFWwindow* window);
        ~controls_();
    }controls;

    glm::mat4 process_controls(controls& context, GLFWwindow* window, float delta_time);

    float get_fov();

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}

#endif /* YZCONTROL_HPP */