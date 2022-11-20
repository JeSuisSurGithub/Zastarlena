#include <yzubo.hpp>

namespace yz
{
namespace memory
{
    ubo::ubo(u32 binding, void* data, usz size)
    {
        glCreateBuffers(1, &m_ubo);
        glNamedBufferStorage(m_ubo, size, data, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
        glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_ubo, 0, size);
    }

    ubo::~ubo()
    {
        glDeleteBuffers(1, &m_ubo);
    }

    void update(ubo& ubo_, void* data, usz size, usz offset)
    {
        void* mapped_buf = glMapNamedBufferRange(ubo_.m_ubo, offset, size, GL_MAP_WRITE_BIT);
        std::memcpy(mapped_buf, data, size);
        glUnmapNamedBuffer(ubo_.m_ubo);
    }
}
}
