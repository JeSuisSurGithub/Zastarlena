#ifndef NO_LIGHT_HPP
#define NO_LIGHT_HPP

#include "yzrendergroup.hpp"
#include <memory>

namespace yz
{
    namespace rendergroups
    {
        typedef struct no_lightgroup_
        {
            no_lightgroup_(const no_lightgroup_ &) = delete;
            no_lightgroup_ &operator=(const no_lightgroup_ &) = delete;
            no_lightgroup_(no_lightgroup_ &&) = delete;
            no_lightgroup_ &operator=(no_lightgroup_ &&) = delete;
            std::unique_ptr<rendergroup> m_base;

            no_lightgroup_();
            ~no_lightgroup_();
        }no_lightgroup;

        void update(no_lightgroup& context, glm::mat4 model, glm::mat4 view, glm::mat4 projection);

        void render(no_lightgroup& context);
    }
}

#endif /* NO_LIGHT_HPP */