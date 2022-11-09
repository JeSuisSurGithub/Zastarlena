#ifndef MAINGROUP_HPP
#define MAINGROUP_HPP

#include "yzcommon.hpp"
#include "yzrendergroup.hpp"

#include <memory>

namespace yz
{
    namespace rendergroups
    {
        typedef struct maingroup_
        {
            maingroup_(const maingroup_ &) = delete;
            maingroup_ &operator=(const maingroup_ &) = delete;
            maingroup_(maingroup_ &&) = delete;
            maingroup_ &operator=(maingroup_ &&) = delete;

            std::unique_ptr<rendergroup> m_base;
            float m_texture_offset_count;

            maingroup_();
            ~maingroup_();
        }maingroup;

        void update(maingroup& context, float delta_time);

        void render(maingroup& context);
    }
}

#endif /* MAINGROUP_HPP */