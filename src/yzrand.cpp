#include <yzrand.hpp>

namespace yz
{
    i32 lehmer_randrange(u32& state, i32 min, i32 max)
    {
        state = (std::uint64_t)state * 279470273u % 0xfffffffb;
        return (state % (max - min)) + min;
    }

    float lehmer_randrange_flt(u32& state, float min, float max)
    {
        state = (std::uint64_t)state * 279470273u % 0xfffffffb;
        return (float)state / (float)(UINT32_MAX / (max - min)) + min;
    }
}