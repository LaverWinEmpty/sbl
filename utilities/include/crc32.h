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
//  CRC32C => use include nmmintrin.h (or immintrin.h, etc... )
//            if not, use CRC32 process
//
//  include check: #ifdef _INCLUDED_NMM

namespace sbl {

class CRC32
{
private:
    static const uint32_t POLY;

private:
    static uint32_t Process(IN UByte* ptr, IN ssize_t size, IN uint32_t init, IN uint32_t poly);

public:
    static uint32_t Hasing(IN Ptr ptr, IN ssize_t size);
    static uint32_t Castagnoli(IN Ptr ptr, IN ssize_t size);
};

} // namespace sbl

#endif