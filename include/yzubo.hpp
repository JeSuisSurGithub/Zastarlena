#ifndef YZUBO_HPP
#define YZUBO_HPP

#include "yzcommon.hpp"

namespace yz
{
    namespace memory
    {
        typedef struct ubo
        {
            ubo(const ubo &) = delete;
            ubo &operator=(const ubo &) = delete;
            ubo(ubo &&) = delete;
            ubo &operator=(ubo &&) = delete;

            GLuint m_ubo;

            ubo(u32 binding, void* data, usz size);
            ~ubo();
        }ubo;

        void update(ubo& ubo_, void* data, usz size, usz offset);

        typedef struct ssbo
        {
            ssbo(const ssbo &) = delete;
            ssbo &operator=(const ssbo &) = delete;
            ssbo(ssbo &&) = delete;
            ssbo &operator=(ssbo &&) = delete;

            GLuint m_ssbo;

            ssbo(u32 binding, void* data, usz size);
            ~ssbo();
        }ssbo;

        void update(ssbo& ssbo_, void* data, usz size, usz offset);
    }
}

#endif /* YZUBO_HPP */