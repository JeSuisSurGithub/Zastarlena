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
    }shader;

    void bind(shader& shader);
    void update_matrix4(shader& program, GLint location, const glm::mat4& matrix);
    void update_uint_array(shader& program, GLint location, GLuint* uintv, std::size_t size);
    void update_int_array(shader& program, GLint location, GLint* intv, std::size_t size);
    void update_uint(shader& program, GLint location, GLuint uint_);
    void update_int(shader& program, GLint location, GLint int_);
    void update_bool(shader& program, GLint location, bool bool_);
    void update_float(shader& program, GLint location, float float_);

    std::vector<u8> read_file(const std::string& filepath);
}

#endif /* YZSHADER_HPP */