#include <yzshader.hpp>

namespace yz
{
    shader_::shader_(const std::string& vertpath, const std::string& fragpath)
    {
        std::vector<u8> vertex_shader_buf = read_file(vertpath);
        GLuint vertex_sid = glCreateShader(GL_VERTEX_SHADER);
        glShaderBinary(1, &vertex_sid, GL_SHADER_BINARY_FORMAT_SPIR_V, vertex_shader_buf.data(), vertex_shader_buf.size());
        glSpecializeShader(vertex_sid, "main", 0, 0, 0);

        int status = 0;
        glGetShaderiv(vertex_sid, GL_COMPILE_STATUS, &status);
        if (!status)
            throw std::runtime_error("Invalid vertex shader\n");

        std::vector<u8> fragment_shader_buf = read_file(fragpath);
        GLuint fragment_sid = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderBinary(1, &fragment_sid, GL_SHADER_BINARY_FORMAT_SPIR_V, fragment_shader_buf.data(), fragment_shader_buf.size());
        glSpecializeShader(fragment_sid, "main", 0, 0, 0);

        status = 0;
        glGetShaderiv(fragment_sid, GL_COMPILE_STATUS, &status);
        if (!status)
            throw std::runtime_error("Invalid fragment shader\n");

        m_id = glCreateProgram();
        glAttachShader(m_id, vertex_sid);
        glAttachShader(m_id, fragment_sid);
        glLinkProgram(m_id);

        status = 0;
        glGetProgramiv(m_id, GL_LINK_STATUS, &status);
        if (!status)
            throw std::runtime_error("Linking failed\n");
        glDeleteShader(vertex_sid);
        glDeleteShader(fragment_sid);
    }

    shader_::~shader_()
    {
        glDeleteProgram(m_id);
    }

    std::vector<u8> read_file(const std::string& filename)
    {
        std::ifstream in_file(filename, std::ios::ate | std::ios::binary);
        if (!in_file.is_open())
            throw std::runtime_error("Failed to open file!");
        std::size_t filesize = in_file.tellg();
        std::vector<u8> buffer(filesize);
        in_file.seekg(0);
        in_file.read(reinterpret_cast<char*>(buffer.data()), filesize);
        in_file.close();
        return buffer;
    }

    void send_matrix4(shader& program, GLint location, float* matrix)
    {
        program.activate();
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
    }

    void send_vec3(shader& program, GLint location, glm::vec3 vec)
    {
        program.activate();
        glUniform3f(location, vec.x, vec.y, vec.z);
    }

    void send_int(shader& program, GLint location, int int_)
    {
        program.activate();
        glUniform1i(location, int_);
    }

    void send_int_array(shader& program, GLint location, GLint* int_array, size_t size)
    {
        program.activate();
        glUniform1iv(location, size, int_array);
    }

    void send_float(shader& program, GLint location, float float_)
    {
        program.activate();
        glUniform1f(location, float_);
    }
}