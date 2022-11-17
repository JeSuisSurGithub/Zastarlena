#include <yzcontrol.hpp>

float g_fov = 45.0;

namespace yz
{
    controls::controls(GLFWwindow* window)
    :
    m_move_speed(5.0),
    m_view_speed(.001),
    m_horizontal_angle(3.14),
    m_vertical_angle(0.0),
    m_show_mouse({.toggled = false, .waited_time = 0, .wait_time = 200, .key =  KEYMAP::SHOW_MOUSE}),
    m_freeze({.toggled = false, .waited_time = 0, .wait_time = 200, .key =  KEYMAP::FREEZE}),
    m_wireframe({.toggled = false, .waited_time = 0, .wait_time = 200, .key =  KEYMAP::WIREFRAME}),
    m_camera_xyz(glm::vec3(0.0, 0.0, 5.0))
    {
        glfwSetScrollCallback(window, scroll_callback);
    }

    controls::~controls() {}

    glm::mat4 process_controls(controls& context, GLFWwindow* window, float delta_time)
    {
        int width{0};
        int height{0};
        glfwGetWindowSize(window, &width, &height);

        bool changed = update_toggle(context.m_show_mouse, window, delta_time);
        update_toggle(context.m_freeze, window, delta_time);
        update_toggle(context.m_wireframe, window, delta_time);

        if (changed)
        {
            glfwSetInputMode(window, GLFW_CURSOR, (context.m_show_mouse.toggled) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
            if (!context.m_show_mouse.toggled)
            {
                glfwSetCursorPos(window, static_cast<double>(width) / 2, static_cast<double>(height) / 2);
            }
        }

        // Movement

        if (!context.m_show_mouse.toggled)
        {
            double xpos{0};
            double ypos{0};
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwSetCursorPos(window, static_cast<double>(width) / 2, static_cast<double>(height) / 2);
            if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
            {
                context.m_horizontal_angle += context.m_view_speed * (g_fov * 0.01) * delta_time * (int(width / 2) - xpos);
                context.m_vertical_angle   += context.m_view_speed * (g_fov * 0.01) * delta_time * (int(height / 2) - ypos);
            }
        }
        glm::vec3 forward(
            cos(context.m_vertical_angle) * sin(context.m_horizontal_angle),
            sin(context.m_vertical_angle),
            cos(context.m_vertical_angle) * cos(context.m_horizontal_angle)
        );
        glm::vec3 right = glm::vec3(
            sin(context.m_horizontal_angle - 3.14/2.0),
            0,
            cos(context.m_horizontal_angle - 3.14/2.0)
        );
        glm::vec3 up = glm::cross(right, forward);

        if (glfwGetKey(window, KEYMAP::INCREASE_SPEED) == GLFW_PRESS)
        {
            context.m_move_speed += 1.0;
        }
        if (glfwGetKey(window, KEYMAP::DECREASE_SPEED) == GLFW_PRESS)
        {
            context.m_move_speed -= 1.0;
        }
        context.m_move_speed = std::clamp<float>(context.m_move_speed, 5.0, 100.0);
        if (glfwGetKey(window, KEYMAP::FORWARD) == GLFW_PRESS)
        {
            context.m_camera_xyz += forward * delta_time * context.m_move_speed;
        }
        if (glfwGetKey(window, KEYMAP::BACKWARD) == GLFW_PRESS)
        {
            context.m_camera_xyz -= forward * delta_time * context.m_move_speed;
        }
        if (glfwGetKey(window, KEYMAP::RIGHT) == GLFW_PRESS)
        {
            context.m_camera_xyz += right * delta_time * context.m_move_speed;
        }
        if (glfwGetKey(window, KEYMAP::LEFT) == GLFW_PRESS)
        {
            context.m_camera_xyz -= right * delta_time * context.m_move_speed;
        }
        return glm::lookAt(context.m_camera_xyz, context.m_camera_xyz + forward, up);
    }

    float get_fov()
    {
        return g_fov;
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        g_fov -= yoffset * 1.3;
        g_fov = std::clamp<float>(g_fov, 1.0, 300.0);
    }

    bool update_toggle(toggle& toggle_, GLFWwindow* window, float delta_time)
    {
        toggle_.waited_time += delta_time;
        if ((toggle_.waited_time >= toggle_.wait_time) && glfwGetKey(window, toggle_.key) == GLFW_PRESS)
        {
            toggle_.waited_time = 0;
            toggle_.toggled = !toggle_.toggled;
            return true;
        }
        return false;
    }
}