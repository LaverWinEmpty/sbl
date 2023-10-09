/*
    Packet: 4kb
    ┌ Packet ─────┬─────────────┬──────────┬─────────────┐
    │ PayloadSize │ PacketIndex │ Protocol │ Payload ... │
    └─────────────┴─────────────┴──────────┴─────────────┘
                                           └─────┬───────┘
                                                 ↓
                        ... | Data ] [ Data Size | Data ] [ DataSize | ...
*/

#ifndef SBL_TCP_HEADER__
#define SBL_TCP_HEADER__

#define _WINSOCKAPI_
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "windows.h"
#include "winsock2.h"
#include "../../container/source/array.hpp"
#include "../../api/include/win.h"

#pragma comment(lib, "ws2_32")

namespace sbl {
namespace iocp {

using DataSize    = int16_t;
using PayloadSize = int16_t;
using PacketIndex = int8_t;
using Protocol    = int64_t;

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
    readonly ssize_t INFO   = sizeof(PayloadSize) + sizeof(Protocol) + sizeof(PacketIndex);
    readonly ssize_t PACKET = DEF_BUF_SIZE;
    readonly ssize_t BUFFER = PACKET - INFO;
};

class SocketTCP
{
    DECLARE_NO_COPY(SocketTCP);

protected:
    SocketTCP();
    SocketTCP(IN const char* ip, IN u_short port);

public:
    ~SocketTCP();

public:
    static SocketTCP* CreateClient(IN const char* ip, IN u_short port);

public:
    SOCKET Accept(OUT OPT SOCKADDR_IN* info = nullptr);
    DWORD  Connect();
    DWORD  Bind();
    DWORD  Listen();

public:
    DWORD TransmitSync(IN Byte* buffer, IN ssize_t size, IN Protocol protocol);
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