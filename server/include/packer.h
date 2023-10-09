#ifndef SBL_PACKER_HEADER__
#define SBL_PACKER_HEADER__

#include "../include/tcp.h"

namespace sbl {
namespace iocp {

// Packing / Unpacking
class Packer
{
public:
    Packer();
    Packer(OUT Ptr buffer, IN DataSize bufferSize);

public:
    void SetBuffer(OUT Ptr buffer, IN DataSize bufferSize);
    bool Packing(IN Ptr data, IN DataSize size);
    bool Unpacking(IN Ptr buffer);
    bool Read(OUT Ptr buffer, IN DataSize size);

public:
    bool PackingInt16(IN int16_t param);
    bool PackingInt32(IN int32_t param);
    bool PackingInt64(IN int64_t param);

public:
    bool UnpackingInt16(OUT int16_t* param);
    bool UnpackingInt32(OUT int32_t* param);
    bool UnpackingInt64(OUT int64_t* param);

public:
    ssize_t GetMoved();
    prop(get = GetMoved) ssize_t Moved;

private:
    Byte*   cursor;
    ssize_t moved;
    ssize_t limit;
};

} // namespace iocp
} // namespace sbl
#endif