#ifndef SBL_CRC_32_HEADER__
#define SBL_CRC_32_HEADER__

#include "../../common/include/common.h"

// About Endian
//   default: little-endian
//   when big-endian environment
//   if __BYTE_ORDER__ / __ORDER_BIG_ENDIAN__ is not defined
//   then recommended to write #define BIG_ENDIAN
//
// About SSE 4.2
//  CRC32  => not use
//  CRC32C => use include nmmintrin.h
//            if not, use CRC32 process

namespace sbl {

class CRC32
{
private:
    static const UInt32 POLY;

private:
    static UInt32 Process(IN UByte* ptr, IN SzInt size, IN UInt32 init, IN UInt32 poly);

public:
    static UInt32 Hasing(IN Ptr ptr, IN SzInt size);
    static UInt32 HasingCRC32C(IN Ptr ptr, IN SzInt size);
};

} // namespace sbl

#endif