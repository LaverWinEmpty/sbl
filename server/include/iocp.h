#ifndef SBL_IOCP_HEADER__
#define SBL_IOCP_HEADER__

#include "tcp.h"

namespace sbl {
namespace iocp {

class SocketState;
class SocketController;

// Overlapped Overwarpped XD
struct AsyncOperation
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
};

// Trans/Recv infomation
struct IOContext
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

class SocketIOCP: public SocketTCP
{
    DECLARE_NO_COPY(SocketIOCP);

private:
    SocketIOCP();

public:
    ~SocketIOCP() {}

public:
    static SocketTCP*  CreateClient(IN const AChar* ip, IN u_short port) = delete;
    static SocketIOCP* CreateServer(IN u_short port);
    static SocketIOCP* CreateSession(IN SOCKET hSocket, IN const SOCKADDR_IN& sockAddr);

public:
    bool          TransmitAsync();
    bool          ReceiveAsync();
    ESocketResult CheckTransmit(IN int completeByte);
    ESocketResult CheckReceive(IN int completeByte);
    void          Packing(IN const Ptr payload, IN DataSize size, IN Protocol protocol);
    bool          Unpacking(OUT OPT Ptr buffer, OUT OPT Protocol* protocol, IN SzInt bufferSize = EPacketSize::PACKET);

public:
    SocketController* GetController() { return ctrl; }

public:
    prop(get = GetController) SocketController* Controller;

private:
    bool              isGotSize = false;    // Size flag
    IOContext         rxInfo;               // Receive buffer
    Queue<IOContext*> txQueue;              // Send buffer => Queue
    AsyncOperation    rxOperate, txOperate; // Overlapped (receive / transmit)

private:
    SocketController* ctrl = nullptr; // No create in listen socket
};

class IOCP abstract
{
public:
    IOCP(IN u_short port);
    ~IOCP();

public:
    void Initialize();

protected:
    static DWORD WINAPI WorkerThread(IN LPVOID iocp);

protected:
    virtual void PostAccept(IN SOCKET hSocket);
    virtual void FailedTransfer()       = 0;
    virtual void Disconnect(IN Ptr arg) = 0;

protected:
    virtual int PostReceive(IN Ptr arg);
    virtual int PostTransmit(IN Ptr arg);

public:
    void AcceptPostQueuedCompletionStatus(IN SOCKET sock);
    void ErrorPostQueuedCompletionStatus(IN SocketIOCP* sock);

protected:
    SocketIOCP* socket = nullptr;
    HANDLE      hcp    = nullptr;
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
    SocketState(IN SocketIOCP* parent);
    virtual ~SocketState();

public:
    bool Receive();
    bool Transmit();

public:
    virtual SocketState* NextState()    = 0; // End: return nullptr
    virtual ENext        PostReceive()  = 0;
    virtual ENext        PostTransmit() = 0;

private:
    SocketIOCP* parent;
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
