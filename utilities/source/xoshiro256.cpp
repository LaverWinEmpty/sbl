#include "../include/xoshiro256.h"

namespace sbl {

Xoshiro256::Xoshiro256(): state{ 1 } {}

Xoshiro256::Xoshiro256(IN uint64_t seed)
{
    Initialize(seed);
}

Xoshiro256::Xoshiro256(IN const Block256& seed)
{
    Initialize(seed);
}

void Xoshiro256::Initialize(IN uint64_t seed)
{
    if(seed == 0) {
        seed = 1;
    }

    else if(CHECK_BIG_ENDIAN()) {
        seed = REVERSE_ENDIAN_64(seed);
    }

    *static_cast<uint64_t*>(state) = seed;
}

void Xoshiro256::Initialize(IN const Block256& seed)
{
    uint64_t* ptr = const_cast<Block256&>(seed);

    readonly int LOOP = sizeof(Block256) / sizeof(uint64_t);

    bool isZero = true;
    for(int i = 0; i < LOOP; ++i) {
        if(ptr[i]) {
            isZero = false;
            break;
        }
    }

    if(isZero) {
        state = { 1 };
        return;
    }

    state = seed;
}

uint64_t Xoshiro256::Generate()
{
    uint64_t* s      = reinterpret_cast<uint64_t*>(&state[0]);
    uint64_t  result = SHIFT_ROT_L_UNSIGNED(s[1] * 5, 7) * 9;
    uint64_t  temp   = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];
    s[2] ^= temp;
    s[3] = SHIFT_ROT_L_UNSIGNED(s[3], 45);

    return result;
}

} // namespace sbl
