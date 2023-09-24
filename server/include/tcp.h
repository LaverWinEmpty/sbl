/*
    Packet: 4kb
    忙式式式式式式式式式式式式式式式式 Info 式式式式式式式式式式式式式式式式忖
    [ PayloadSize | PacketIndex | Protocol | Payload ... ]
                                                ⊿
                        ... | Data ] [ Data Size | Data ] [ DataSize | ...
*/

#ifndef __SEMIBASE_WINDOWS_IOCP_HEADER__
#define __SEMIBASE_WINDOWS_IOCP_HEADER__

#define _WINSOCKAPI_
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "windows.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32")

#include "../../container/source/array.hpp"

namespace sbl {
namespace iocp {

using DataSize    = Int16;
using PayloadSize = Int16;
using PacketIndex = Int8;
using Protocol    = Int64;

enum class ESocketResult
{
    SOC_ERROR    = 1,
    SOC_TRUE     = 2,
    SOC_FALSE    = 3,
    SOC_CONTINUE = 4,
};

class EPacketSize
{
    DECLARE_LIMIT_LIFECYCLE(EPacketSize);

public:
    readonly SzInt INFO   = sizeof(PayloadSize) + sizeof(Protocol) + sizeof(PacketIndex);
    readonly SzInt PACKET = DEF_BUF_SIZE;
    readonly SzInt BUFFER = PACKET - INFO;
};

class SocketTCP
{
    DECLARE_NO_COPY(SocketTCP);

protected:
    SocketTCP();

public:
    ~SocketTCP();

public:
    static SocketTCP* CreateClient(IN const AChar* ip, IN u_short port);

public:
    SOCKET Accept(OUT OPT SOCKADDR_IN* info = nullptr);
    DWORD  Connect();
    DWORD  Bind();
    DWORD  Listen();

public:
    DWORD TransmitSync(IN Byte* buffer, IN SzInt size, IN Protocol protocol);
    DWORD ReceiveSync(OUT OPT Byte* buffer, OUT OPT Protocol* protocol, OUT OPT DataSize* received = nullptr);

public:
    void        NextIndex();
    PacketIndex GetPacketNumber() const;
    SOCKET      GetSocketHandle() const;

public:
    prop(get = GetSocketHandle) SOCKET Handle;
    prop(get = GetPacketNumber) PacketIndex PacketNumber;

protected:
    SOCKET      handle = NULL;
    SOCKADDR_IN info   = { 0 };

protected:
    PacketIndex index = 0; // Packet order
};

} // namespace iocp
} // namespace sbl
#endif