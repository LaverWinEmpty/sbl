#include "../include/xoshiro256.h"

namespace sbl {

Xoshiro256::Xoshiro256(): state{ 1 } {}

Xoshiro256::Xoshiro256(IN UInt64 seed)
{
    Initialize(seed);
}

Xoshiro256::Xoshiro256(IN const Block256& seed)
{
    Initialize(seed);
}

void Xoshiro256::Initialize(IN UInt64 seed)
{
    if(seed == 0) {
        seed = 1;
    }

    else if(CHECK_BIG_ENDIAN()) {
        seed = REVERSE_ENDIAN_64(seed);
    }

    *static_cast<UInt64*>(state) = seed;
}

void Xoshiro256::Initialize(IN const Block256& seed)
{
    UInt64* ptr = const_cast<Block256&>(seed);

    readonly int LOOP = sizeof(Block256) / sizeof(UInt64);

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

UInt64 Xoshiro256::Generate()
{
    UInt64* s      = reinterpret_cast<UInt64*>(&state[0]);
    UInt64  result = SHIFT_ROT_L_UNSIGNED(s[1] * 5, 7) * 9;
    UInt64  temp   = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];
    s[2] ^= temp;
    s[3] = SHIFT_ROT_L_UNSIGNED(s[3], 45);

    return result;
}

} // namespace sbl
