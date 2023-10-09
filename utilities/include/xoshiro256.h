#ifndef SBL_XOSHIRO256_HEADER__
#define SBL_XOSHIRO256_HEADER__

#include "../../common/include/common.h"

namespace sbl {

class Xoshiro256
{
public:
    Xoshiro256();
    Xoshiro256(IN uint64_t seed);
    Xoshiro256(IN const Block256& seed);

public:
    void     Initialize(IN uint64_t seed);
    void     Initialize(IN const Block256& seed);
    uint64_t Generate();

private:
    Block256 state;
};

} // namespace sbl

#endif