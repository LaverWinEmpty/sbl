#include "../include/crc32.h"

namespace sbl {

#ifdef BIG_ENDIAN
const UInt32 CRC32::POLY = 0x04C11DB7;
#else
const UInt32 CRC32::POLY = 0xEDB88320;
#endif

UInt32 CRC32::Process(IN UByte* ptr, IN SzInt size, IN UInt32 init, IN UInt32 poly)
{
    while(size-- > 0) {
        init ^= *ptr++;
        for(int i = 0; i < 8; ++i) {
            init = (init >> 1) ^ (poly & (0 - (init & 1)));
        }
    }
    return ~init;
}

UInt32 CRC32::Hasing(IN Ptr ptr, IN SzInt size)
{
    return Process(ptr, size, -1, POLY);
}

UInt32 CRC32::HasingCRC32C(const Ptr ptr, SzInt size)
{
    unsigned char* data = ptr;
    unsigned int   crc  = -1;

#ifndef _INCLUDED_NMM
#    ifdef BIG_ENDIAN
    readonly UInt32 POLY = 0x1EDC6F41;
#    else
    readonly UInt32 POLY = 0x82F63B78;
#    endif
    return Process(data, size, crc, POLY);
#else

    while(size >= 16) {
        __m128i chunk = _mm_loadu_si128((__m128i*)data); // copy for SSE 4.2: 128 bit / 32 bit => 4 loop

        crc = _mm_crc32_u32(crc, _mm_extract_epi32(chunk, 0));
        crc = _mm_crc32_u32(crc, _mm_extract_epi32(chunk, 1));
        crc = _mm_crc32_u32(crc, _mm_extract_epi32(chunk, 2));
        crc = _mm_crc32_u32(crc, _mm_extract_epi32(chunk, 3));

        data += 16;
        size -= 16;
    }

    // Process the remaining 4 ~ 15 bytes
    while(size >= 4) {
        crc = _mm_crc32_u32(crc, *reinterpret_cast<unsigned int*>(data));
        data += 4;
        size -= 4;
    }

    // Process the remaining 0 ~ 3 bytes
    while(size-- > 0) {
        crc = _mm_crc32_u8(crc, *data++);
    }

    return ~crc;
#endif
}

} // namespace sbl