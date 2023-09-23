#ifndef __SEMIBASE_HASH_SPOOKY_V2_HEADER__
#define __SEMIBASE_HASH_SPOOKY_V2_HEADER__

#include "cstring"
#include "../../common/include/common.h"
#include "../../common/include/int128.h"

namespace sbl {

class SpookyV2
{
private:
    readonly SzInt  NUM_OF_STATE = 12;
    readonly SzInt  BLOCK_SIZE   = NUM_OF_STATE * 8;
    readonly SzInt  BUFFER_SIZE  = BLOCK_SIZE * 2;
    readonly UInt64 CONSTANT     = 0xDEADBEEFDEADBEEF;

private:
    class Mixer
    {
    public:
        Mixer();
        UInt64& operator[](IN SzInt index);
        void    Init(IN const SpookyV2& parent);
        void    Set(IN const SpookyV2& parent);
        void    Mix(const UInt64* data);
        void    End(const UInt64* data);
    private:
        UInt64 state[NUM_OF_STATE];
    };

public:
    SpookyV2();
    SpookyV2(IN UInt128 seed);

private:
    void    Initialize();
    void    Update(IN const Ptr ptr, IN SzInt size);
    UInt128 Final();

public:
    void Initialize(IN UInt128 seed);

public:
    UInt128 Hash128(IN const Ptr ptr, IN SzInt size);
    UInt64  Hash64(IN const Ptr ptr, IN SzInt size);
    UInt32  Hash32(IN const Ptr ptr, IN SzInt size);

private:
    UInt128 seed;
    UInt64  unhashed[NUM_OF_STATE * 2];
    UInt64  state[NUM_OF_STATE];
    SzInt   length;
    SzInt   remaind;
};

} // namespace sbl
#endif