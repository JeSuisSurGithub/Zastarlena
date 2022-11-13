#ifndef YZRAND_HPP
#define YZRAND_HPP

#include "yzcommon.hpp"

namespace yz
{
    i32 lehmer_randrange(u32& state, i32 min, i32 max);
    float lehmer_randrange_flt(u32& state, float min, float max);
}

#endif /* YZRAND_HPP */