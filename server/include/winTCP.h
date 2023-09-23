#ifndef __SEMIBASE_WINDOWS_SOCKET_TCP_HEADER__
#define __SEMIBASE_WINDOWS_SOCKET_TCP_HEADER__

#define _WINSOCKAPI_
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "windows.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32")

#include "../../container/source/array.hpp"
#include "server.h"

namespace sbl {
namespace iocp {

using DataSize    = Int16;
using PayloadSize = Int16;
using PacketIndex = Int8;
using Protocol    = Int64;

class SocketBuilder;
class SocketTCP;
class SocketController;

enum class ESocketResult
{
    SOC_ERROR    = 1,
    SOC_TRUE     = 2,
    SOC_FALSE    = 3,
    SOC_CONTINUE = 4,
};

/*
    Packet: 4kb
    忙式式式式式式式式式式式式式式式式 Info 式式式式式式式式式式式式式式式式忖
    [ PayloadSize | PacketIndex | Protocol | Payload ... ]
                                                ⊿
                        ... | Data ] [ Data Size | Data ] [ DataSize | ...
*/

class EPacketSize
{
    DECLARE_LIMIT_LIFECYCLE(EPacketSize);

public:
    readonly SzInt INFO   = sizeof(PayloadSize) + sizeof(Protocol) + sizeof(PacketIndex);
    readonly SzInt PACKET = DEF_BUF_SIZE;
    readonly SzInt BUFFER = PACKET - INFO;
};

// Overlapped Overwarpped XD
typedef struct AsyncOperation
{
    enum EType
    {
        IO_ERROR        = 0,
        IO_ACCEPT       = 1,
        IO_READ         = 2,
        IO_WRITE        = 3,
        IO_DISCONNECTED = 4,
    };

    WSAOVERLAPPED async  = { 0 };
    EType         type   = EType::IO_ERROR;
    Ptr           parent = nullptr;
}* PtrAsync;

// Trans/Recv infomation
typedef struct IOContext
{
    DECLARE_NO_COPY(IOContext);

    WSABUF cursor;
    int    current;
    int    target;
    Byte*  buffer;

    IOContext(): cursor{ 0 }, current(0), target(0), buffer(nullptr) {}
    IOContext(SzInt size): cursor{ 0 }, current(0), target(0), buffer(nullptr) { buffer = new Byte[size]{ 0 }; }
    ~IOContext() { SAFE_DESTRUCTS(buffer); }
};

class SocketTCP
{
    DECLARE_NO_COPY(SocketTCP);

    friend SocketBuilder;

private:
    SocketTCP();

public:
    ~SocketTCP() {}

public:
    SOCKET Accept(OUT OPT SOCKADDR_IN* info = nullptr);
    DWORD  Connect();
    DWORD  Bind();
    DWORD  Listen();

public:
    DWORD Transmit(IN Byte* buffer, IN SzInt size, IN Protocol protocol);
    DWORD Receive(OUT OPT Byte* buffer, OUT OPT Protocol* protocol, OUT OPT DataSize* received = nullptr);

public:
    bool          Transmit(); // IOCP
    bool          Receive();  // IOCP
    ESocketResult CheckTransmit(IN int completeByte);
    ESocketResult CheckReceive(IN int completeByte);
    void          Packing(IN const Ptr payload, IN DataSize size, IN Protocol protocol);
    bool          Unpacking(OUT OPT Ptr buffer, OUT OPT Protocol* protocol, IN SzInt bufferSize = EPacketSize::PACKET);

public:
    void              NextIndex() { ++index; }
    SOCKET            GetSocketHandle() const { return handle; }
    PacketIndex       GetSequential() const { return index; }
    SocketController* GetFSM() { return fsm; }

public:
    prop(get = GetSocketHandle) SOCKET Handle;
    prop(get = GetSequential) PacketIndex CurrentPacketIndex;
    prop(get = GetFSM) SocketController* Controller;

private:
    SocketController* fsm = nullptr; // No create in listen socket

private:
    SOCKET      handle = NULL;
    SOCKADDR_IN info   = { 0 };

private:
    bool              isGotSize = false;    // Size flag
    PacketIndex       index     = 0;        // Packet order
    IOContext         rxInfo;               // Receive buffer
    Queue<IOContext*> txQueue;              // Send buffer => Queue
    AsyncOperation    rxOperate, txOperate; // Overlapped (receive / transmit)
};

class SocketBuilder
{
    DECLARE_LIMIT_LIFECYCLE(SocketBuilder);

public:
    static SocketTCP* Server(IN u_short port);
    static SocketTCP* Client(IN const AChar* ip, IN u_short port);
    static SocketTCP* Session(IN SOCKET hSocket, IN const SOCKADDR_IN& sockAddr);
};

// Abstract
class SocketState abstract
{
public:
    enum ENext : bool
    {
        TRANSMIT,
        RECEIVE,
    };

public:
    SocketState(IN SocketTCP* parent);
    virtual ~SocketState();

public:
    bool Receive();
    bool Transmit();

public:
    virtual SocketState* NextState()    = 0; // End: return nullptr
    virtual ENext        PostReceive()  = 0;
    virtual ENext        PostTransmit() = 0;

private:
    SocketTCP* parent;
};

// State Machine
class SocketController
{
public:
    SocketController();
    SocketController(SocketState* param);
    ~SocketController();

public:
    bool Receive();
    bool Transmit();

public:
    SocketState::ENext PostReceive();
    SocketState::ENext PostTransmit();

public:
    SocketState* GetNext();
    bool         IsSet();
    void         SetState(IN SocketState* state);
    void         ChangeState();

private:
    SocketState* state;
};

} // namespace iocp
} // namespace sbl
#endif