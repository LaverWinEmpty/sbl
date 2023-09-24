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
    bool PackingInt16(IN Int16 param);
    bool PackingInt32(IN Int32 param);
    bool PackingInt64(IN Int64 param);

public:
    bool UnpackingInt16(OUT Int16* param);
    bool UnpackingInt32(OUT Int32* param);
    bool UnpackingInt64(OUT Int64* param);

public:
    SzInt GetMoved();
    prop(get = GetMoved) SzInt Moved;

private:
    Byte* cursor;
    SzInt moved;
    SzInt limit;
};

} // namespace iocp
} // namespace sbl
#endif