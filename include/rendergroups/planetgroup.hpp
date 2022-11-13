#ifndef PLANETGROUP_HPP
#define PLANETGROUP_HPP

#include "yzrendergroup.hpp"
#include <memory>

namespace yz
{
    namespace rendergroups
    {
        typedef struct planetgroup
        {
            planetgroup(const planetgroup &) = delete;
            planetgroup &operator=(const planetgroup &) = delete;
            planetgroup(planetgroup &&) = delete;
            planetgroup &operator=(planetgroup &&) = delete;

            std::unique_ptr<rendergroup> m_base;

            planetgroup();
            ~planetgroup();
        }planetgroup;

        void update(planetgroup& context, float delta_time);
        void render(planetgroup& context);
    }
}

#endif /* PLANETGROUP_HPP */