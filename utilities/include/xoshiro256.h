#ifndef SBL_XOSHIRO_256_HEADER__
#define SBL_XOSHIRO_256_HEADER__

#include "../../common/include/common.h"
#include "../../common/include/block.h"

namespace sbl {

class Xoshiro256
{
public:
    Xoshiro256();
    Xoshiro256(IN UInt64 seed);
    Xoshiro256(IN const Block256& seed);

public:
    void   Initialize(IN UInt64 seed);
    void   Initialize(IN const Block256& seed);
    UInt64 Generate();

private:
    Block256 state;
};

} // namespace sbl

#endif