#ifndef YZGEN_HPP
#define YZGEN_HPP

#include "yzcommon.hpp"
#include "rendergroups/planetgroup.hpp"
#include "rendergroups/stargroup.hpp"

namespace yz
{
    typedef struct planet_info
    {
        float planet_distance_to_star;
        float planet_revolution_speed;
        float planet_orbital_speed;
        float cur_angle;
    }planet_info;

    typedef struct gen_context
    {
        gen_context(const gen_context &) = delete;
        gen_context &operator=(const gen_context &) = delete;
        gen_context(gen_context &&) = delete;
        gen_context &operator=(gen_context &&) = delete;

        std::vector<usz> m_star_planet_count;
        std::vector<planet_info> m_planets;

        gen_context(
            rendergroups::stargroup& stars,
            rendergroups::planetgroup& planets,
            ubo_point_light* point_lights,
            usz& point_light_count,
            u32 seed,
            usz count);
        ~gen_context();
    }gen_context;

    i32 lehmer_randrange(u32& state, i32 min, i32 max);
    float lehmer_randrange_flt(u32& state, float min, float max);

    void update(
        gen_context& context,
        float delta_time,
        rendergroups::stargroup& stars,
        rendergroups::planetgroup& planets);
}

#endif /* YZGEN_HPP */