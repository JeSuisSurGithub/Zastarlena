#ifndef YZCONTROL_HPP
#define YZCONTROL_HPP

#include "yzcommon.hpp"
#include <GLFW/glfw3.h>

namespace yz
{
    typedef enum KEYMAP
    {
        FORWARD     = GLFW_KEY_W,
        LEFT        = GLFW_KEY_A,
        BACKWARD    = GLFW_KEY_S,
        RIGHT       = GLFW_KEY_D,
        EXIT        = GLFW_KEY_ESCAPE,
        SHOW_MOUSE  = GLFW_KEY_LEFT_ALT,
        SPEED       = GLFW_KEY_LEFT_SHIFT,
        FREEZE      = GLFW_KEY_SPACE,
        WIREFRAME   = GLFW_KEY_LEFT_CONTROL,
        INCREASE_SPEED  = GLFW_KEY_UP,
        DECREASE_SPEED  = GLFW_KEY_DOWN,
    }KEYMAP;

    typedef struct toggle
    {
        bool toggled;
        u32 waited_time;
        u32 wait_time;
        KEYMAP key;
    }toggle;

    typedef struct controls
    {
        controls(const controls &) = delete;
        controls &operator=(const controls &) = delete;
        controls(controls &&) = delete;
        controls &operator=(controls &&) = delete;

        float m_move_speed;
        float m_view_speed;
        float m_horizontal_angle;
        float m_vertical_angle;

        toggle m_show_mouse;
        toggle m_freeze;
        toggle m_wireframe;
        glm::vec3 m_camera_xyz;

        controls(GLFWwindow* window);
        ~controls();
    }controls;

    glm::mat4 process_controls(controls& context, GLFWwindow* window, float delta_time);

    float get_fov();

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    bool update_toggle(toggle& toggle_, GLFWwindow* window, float delta_time);
}

#endif /* YZCONTROL_HPP */