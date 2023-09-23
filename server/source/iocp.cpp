#include "../include/iocp.h"

namespace sbl {
namespace iocp {

DWORD WINAPI IOCP::WorkerThread(IN LPVOID iocp)
{
    int   retVal;
    IOCP* server = reinterpret_cast<IOCP*>(iocp);

    while(1) {
        DWORD    cbTransferred;
        SOCKET   clientSock;
        PtrAsync operation;

        retVal = GetQueuedCompletionStatus(server->hcp,
                                           &cbTransferred,
                                           (ULONG_PTR*)&clientSock,
                                           (LPOVERLAPPED*)&operation,
                                           INFINITE);

        SocketTCP*            ptr     = static_cast<SocketTCP*>(operation->parent);
        AsyncOperation::EType ioType  = static_cast<AsyncOperation::EType>(operation->type);
        WSAOVERLAPPED*        overlap = &operation->async;

        if(retVal == 0 || cbTransferred == 0) {
            if(retVal == 0) {
                DWORD temp1, temp2;
                WSAGetOverlappedResult(clientSock, overlap, &temp1, FALSE, &temp2);
                server->FailedTransfer();
            }

            ioType = AsyncOperation::IO_DISCONNECTED;
        }

        switch(ioType) {
            case AsyncOperation::IO_ACCEPT:
                server->PostAccept(clientSock);
                delete operation;
                break;

            case AsyncOperation::IO_READ:
                switch(ptr->CheckReceive(cbTransferred)) {
                    case ESocketResult::SOC_ERROR: server->ErrorPostQueuedCompletionStatus(ptr);
                    case ESocketResult::SOC_FALSE: continue;
                    case ESocketResult::SOC_TRUE: break;
                    default: continue;
                }

                server->PostReceive(ptr);
                break;

            case AsyncOperation::IO_WRITE:
                switch(ptr->CheckTransmit(cbTransferred)) {
                    case ESocketResult::SOC_ERROR: server->ErrorPostQueuedCompletionStatus(ptr);
                    case ESocketResult::SOC_FALSE:
                    case ESocketResult::SOC_CONTINUE: continue;

                    case ESocketResult::SOC_TRUE: break;

                    default: continue;
                }

                server->PostTransmit(ptr);
                break;

            case AsyncOperation::IO_ERROR: delete operation;
            case AsyncOperation::IO_DISCONNECTED: server->Disconnect(ptr); break;
        }
    }

    return 0;
}

// Throw ErrMsg
IOCP::IOCP()
{
    WSADATA wsa;
    if(0 != WSAStartup(MAKEWORD(2, 2), &wsa)) {
        throw GetLastError();
    }

    socket = SocketBuilder::Server(SERVER_PORT);
    socket->Bind();
    socket->Listen();

    throw ErrorBuilder::NewFailed();
}

IOCP::~IOCP()
{
    SAFE_DELETE(socket);
    WSACleanup();
}

// Throw ErrMsg
void IOCP::Initialize()
{
    hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if(hcp == NULL) {
        throw ErrorBuilder::CreateFailed();
    }

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    // CPU core amount * 2
    HANDLE hThread;
    for(int i = 0; i < (int)si.dwNumberOfProcessors << 1; ++i) {
        hThread = CreateThread(NULL, 0, WorkerThread, this, 0, NULL);
        if(hThread == NULL) {
            throw ErrorBuilder::CreateFailed();
        }
        CloseHandle(hThread);
    }
}

void IOCP::PostAccept(IN SOCKET hSocket)
{
    CreateIoCompletionPort((HANDLE)hSocket, hcp, hSocket, 0);
}

void IOCP::AcceptPostQueuedCompletionStatus(IN SOCKET sock)
{
    PtrAsync overlapped = new AsyncOperation;

    overlapped->type   = AsyncOperation::IO_ACCEPT;
    overlapped->parent = (Ptr*)sock;

    PostQueuedCompletionStatus(hcp, TO_INT(AsyncOperation::IO_ACCEPT), sock, (LPOVERLAPPED)overlapped);
}

void IOCP::ErrorPostQueuedCompletionStatus(IN SocketTCP* sock)
{
    PtrAsync overlapped = new AsyncOperation;

    overlapped->type   = AsyncOperation::IO_DISCONNECTED;
    overlapped->parent = (Ptr*)sock;

    PostQueuedCompletionStatus(hcp, TO_INT(AsyncOperation::IO_ERROR), sock->Handle, (LPOVERLAPPED)overlapped);
}

int IOCP::PostReceive(IN Ptr arg)
{
    using ESocNext = SocketState::ENext;

    SocketTCP*        client = arg;
    SocketController* ctrl   = client->Controller;
    ESocNext          next   = ctrl->PostReceive();

    ctrl->ChangeState();
    if(ctrl->IsSet()) {
        switch(next) {
            case ESocNext::RECEIVE: ctrl->Receive(); break;
            case ESocNext::TRANSMIT: ctrl->Transmit(); break;
        }
    }

    return 0;
}

int IOCP::PostTransmit(IN Ptr arg)
{
    using ESocNext = SocketState::ENext;

    SocketTCP*        client = arg;
    SocketController* ctrl   = client->Controller;
    ESocNext          next   = ctrl->PostTransmit();

    ctrl->ChangeState();
    if(ctrl->IsSet()) {
        switch(next) {
            case ESocNext::RECEIVE: ctrl->Receive(); break;
            case ESocNext::TRANSMIT: ctrl->Transmit(); break;
        }
    }

    return 0;
}

} // namespace iocp
} // namespace sbl