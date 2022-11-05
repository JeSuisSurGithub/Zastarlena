#ifndef YZSHADER_HPP
#define YZSHADER_HPP

#include "yzcommon.hpp"

#include <fstream>
#include <vector>

namespace yz
{
    typedef struct shader_
    {
        shader_(const shader_ &) = delete;
        shader_ &operator=(const shader_ &) = delete;
        shader_(shader_ &&) = delete;
        shader_ &operator=(shader_ &&) = delete;

        GLuint m_id;
        shader_(const std::string&, const std::string&);
        ~shader_();

        void activate() { glUseProgram(m_id); }
    }shader;

    void send_matrix4(shader& program, GLint location, float* matrix);

    void send_vec3(shader& program, GLint location, glm::vec3 vec);

    void send_int(shader& program, GLint location, int int_);

    void send_int_array(shader& program, GLint location, GLint* int_array, size_t size);
    void send_float(shader& program, GLint location, float float_);

    std::vector<u8> read_file(const std::string&);
}

#endif /* YZSHADER_HPP */