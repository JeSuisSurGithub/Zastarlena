#include <yzubo.hpp>

namespace yz
{
namespace memory
{
    ubo::ubo(u32 binding, void* data, usz size)
    {
        glCreateBuffers(1, &m_ubo);
        glNamedBufferStorage(m_ubo, size, data, GL_DYNAMIC_STORAGE_BIT);
        glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_ubo, 0, size);
    }

    ubo::~ubo()
    {
        glDeleteBuffers(1, &m_ubo);
    }

    void update(ubo& ubo_, void* data, usz size, usz offset)
    {
        glNamedBufferSubData(ubo_.m_ubo, offset, size, (u8*)data + offset);
    }

    ssbo::ssbo(u32 binding, void* data, usz size)
    {
        glCreateBuffers(1, &m_ssbo);
        glNamedBufferStorage(m_ssbo, size, data, GL_DYNAMIC_STORAGE_BIT);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, m_ssbo, 0, size);
    }

    ssbo::~ssbo()
    {
        glDeleteBuffers(1, &m_ssbo);
    }

    void update(ssbo& ssbo_, void* data, usz size, usz offset)
    {
        glNamedBufferSubData(ssbo_.m_ssbo, offset, size, (u8*)data + offset);
    }
}
}
