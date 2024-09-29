#ifndef ZSLGEN_HPP
#define ZSLGEN_HPP

#include "common.hpp"
#include "rendergroups/planetgroup.hpp"
#include "rendergroups/stargroup.hpp"

namespace zsl
{
    i32 lehmer_randrange(u32& state, i32 min, i32 max);
    float lehmer_randrange_flt(u32& state, float min, float max);
    glm::vec3 lehmer_randrange_vec3(u32& state, glm::vec3 min, glm::vec3 max);

    void generate(
        rendergroups::stargroup& stars,
        rendergroups::planetgroup& planets,
        u32 seed,
        usz count);
}

#endif /* ZSLGEN_HPP */