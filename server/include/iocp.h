#ifndef __SEMIBASE_WINDOWS_IOCP_HEADER__
#define __SEMIBASE_WINDOWS_IOCP_HEADER__

#include "winTCP.h"

namespace sbl {
namespace iocp {

typedef class IOCP abstract
{
public:
    IOCP();
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
    void ErrorPostQueuedCompletionStatus(IN SocketTCP* sock);

protected:
    SocketTCP* socket = nullptr;
    HANDLE     hcp    = nullptr;
}* PtrIOCP;

} // namespace iocp
} // namespace sbl
#endif