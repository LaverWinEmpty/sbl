#include "../include/packer.h"

namespace sbl {
namespace iocp {

Packer::Packer(): cursor(nullptr), limit(0), moved(0) {}

// Packing: Size of buffer to serialize
// Unpacking: Size of serialized buffer
Packer::Packer(OUT Ptr buffer, IN DataSize bufferSize): cursor(buffer), limit(bufferSize), moved(0) {}

// Packing: Size of buffer to serialize
// Unpacking: Size of serialized buffer
void Packer::SetBuffer(OUT Ptr buffer, IN DataSize bufferSize)
{
    cursor = buffer;
    limit  = bufferSize;
    moved  = 0;
}

// False: Overflow
bool Packer::Packing(IN Ptr data, IN DataSize size)
{
    DataSize after = moved + sizeof(DataSize) + size;
    if(after >= limit) {
        return false;
    }

    // Byte align
    DataSize sizeByteAlign = size;
    if(CHECK_BIG_ENDIAN()) {
        int   last   = sizeof(DataSize) - 1;
        Byte* before = reinterpret_cast<Byte*>(&size);
        Byte* after  = reinterpret_cast<Byte*>(&sizeByteAlign);

        for(int i = 0; i < sizeof(DataSize); ++i) {
            after[i] = before[last - i];
        }
    }

    // Packing
    *(DataSize*)cursor = sizeByteAlign;
    cursor += sizeof(DataSize);
    memcpy(cursor, data, size);
    cursor += size;
    moved = after;
    return true;
}

// False: Read end
bool Packer::Unpacking(IN Ptr buffer)
{
    if(moved + sizeof(DataSize) >= limit) {
        return false;
    }

    // Get size
    DataSize size = *(DataSize*)cursor;
    cursor += sizeof(DataSize);

    // Byte align
    DataSize sizeByteAlign = size;
    if(CHECK_BIG_ENDIAN()) {
        int   last   = sizeof(DataSize) - 1;
        Byte* before = reinterpret_cast<Byte*>(&size);
        Byte* after  = reinterpret_cast<Byte*>(&sizeByteAlign);

        for(int i = 0; i < sizeof(DataSize); ++i) {
            after[i] = before[last - i];
        }
    }

    // Unpacking
    memcpy(buffer, cursor, sizeByteAlign);
    cursor += sizeByteAlign;

    return true;
}

// False: Read end
bool Packer::Read(OUT Ptr buffer, IN DataSize size)
{
    if(moved + sizeof(DataSize) >= limit) {
        return false;
    }

    memcpy(buffer, cursor, size);
    cursor += size;

    return true;
}

bool Packer::PackingInt16(IN Int16 param)
{
    if(CHECK_BIG_ENDIAN()) {
        param = ntohs(param);
    }
    return Packing(&param, sizeof(Int16));
}

bool Packer::PackingInt32(IN Int32 param)
{
    if(CHECK_BIG_ENDIAN()) {
        param = ntohl(param);
    }
    return Packing(&param, sizeof(Int32));
}

bool Packer::PackingInt64(IN Int64 param)
{
    if(CHECK_BIG_ENDIAN()) {
        param = ntohll(param);
    }
    return Packing(&param, sizeof(Int64));
}

bool Packer::UnpackingInt16(OUT Int16* param)
{
    if(Unpacking(param)) {
        return false;
    }
    if(CHECK_BIG_ENDIAN()) {
        *param = ntohs(*param);
    }
    return true;
}

bool Packer::UnpackingInt32(OUT Int32* param)
{
    if(false == Unpacking(param)) {
        return false;
    }
    if(CHECK_BIG_ENDIAN()) {
        *param = ntohl(*param);
    }
    return true;
}

bool Packer::UnpackingInt64(OUT Int64* param)
{
    if(false == Unpacking(param)) {
        return false;
    }

    if(CHECK_BIG_ENDIAN()) {
        *param = ntohll(*param);
    }
    return true;
}

SzInt Packer::GetMoved()
{
    return moved;
}

} // namespace iocp
} // namespace sbl