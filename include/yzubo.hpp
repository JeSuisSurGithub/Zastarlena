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
    }
}

#endif /* YZUBO_HPP */