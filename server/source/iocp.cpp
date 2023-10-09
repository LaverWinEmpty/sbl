#include "../include/iocp.h"

namespace sbl {
namespace iocp {

DWORD WINAPI IOCP::WorkerThread(IN LPVOID iocp)
{
    int   retVal;
    IOCP* server = reinterpret_cast<IOCP*>(iocp);

    while(1) {
        DWORD           cbTransferred;
        SOCKET          clientSock;
        AsyncOperation* operation;

        retVal = GetQueuedCompletionStatus(server->hcp,
                                           &cbTransferred,
                                           (ULONG_PTR*)&clientSock,
                                           (LPOVERLAPPED*)&operation,
                                           INFINITE);

        SocketIOCP*           ptr     = static_cast<SocketIOCP*>(operation->parent);
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

// throw
// runtime_error: WSAtartup()
IOCP::IOCP(IN u_short port)
{
    WSADATA wsa;
    if(0 != WSAStartup(MAKEWORD(2, 2), &wsa)) {
        throw win::ErrorFormatter::Formatting(GetLastError());
    }

    socket = SocketIOCP::CreateServer(port);
    socket->Bind();
    socket->Listen();
}

IOCP::~IOCP()
{
    SAFE_DELETE(socket);
    WSACleanup();
}

// throw
// runtime_error: CreateIoCompletionPort
// runtime_error: CreateThread
void IOCP::Initialize()
{
    hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if(hcp == NULL) {
        throw win::ErrorFormatter::Formatting(GetLastError());
    }

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    // CPU core amount * 2
    HANDLE hThread;
    for(int i = 0; i < (int)si.dwNumberOfProcessors << 1; ++i) {
        hThread = CreateThread(NULL, 0, WorkerThread, this, 0, NULL);
        if(hThread == NULL) {
            throw win::ErrorFormatter::Formatting(GetLastError());
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
    AsyncOperation* overlapped = new AsyncOperation;

    overlapped->type   = AsyncOperation::IO_ACCEPT;
    overlapped->parent = (Ptr*)sock;

    PostQueuedCompletionStatus(hcp, TO_INT(AsyncOperation::IO_ACCEPT), sock, (LPOVERLAPPED)overlapped);
}

void IOCP::ErrorPostQueuedCompletionStatus(IN SocketIOCP* sock)
{
    AsyncOperation* overlapped = new AsyncOperation;

    overlapped->type   = AsyncOperation::IO_DISCONNECTED;
    overlapped->parent = (Ptr*)sock;

    PostQueuedCompletionStatus(hcp, TO_INT(AsyncOperation::IO_ERROR), sock->Handle, (LPOVERLAPPED)overlapped);
}

int IOCP::PostReceive(IN Ptr arg)
{
    using ESocNext = SocketState::ENext;

    SocketIOCP*       client = arg;
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

    SocketIOCP*       client = arg;
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

SocketIOCP::SocketIOCP(): SocketTCP()
{
    rxOperate.parent = this;
    txOperate.parent = this;
    txOperate.type   = AsyncOperation::IO_WRITE;
    rxOperate.type   = AsyncOperation::IO_READ;
}

bool SocketIOCP::TransmitAsync()
{
    DWORD      dummy;
    IOContext* txData = txQueue.Front();

    // Initialize
    ZeroMemory(&txOperate, sizeof(WSAOVERLAPPED));
    txData->cursor.buf = reinterpret_cast<char*>(txData->buffer + txData->current); // Init => + 0
    txData->cursor.len = txData->target - txData->current;                          // Init => + 0

    if(SOCKET_ERROR ==
       WSASend(handle, &txData->cursor, 1, &dummy, 0, reinterpret_cast<LPWSAOVERLAPPED>(&txOperate), nullptr)) {
        if(WSAGetLastError() != WSA_IO_PENDING) {
            return false;
        }
    }
    return true;
}

bool SocketIOCP::ReceiveAsync()
{
    DWORD dummy, flag = 0;

    // Initilaize
    ZeroMemory(&rxOperate, sizeof(WSAOVERLAPPED));
    rxInfo.cursor.buf = reinterpret_cast<char*>(rxInfo.buffer) + rxInfo.current;

    // Get remaind
    if(isGotSize == true) {
        rxInfo.cursor.len = rxInfo.target - rxInfo.current; // Payload data
    }
    else {
        rxInfo.cursor.len = sizeof(PayloadSize) - rxInfo.current; // Payload size
    }

    if(SOCKET_ERROR ==
       WSARecv(handle, &rxInfo.cursor, 1, &dummy, &flag, reinterpret_cast<LPWSAOVERLAPPED>(&rxOperate), nullptr)) {
        if(WSAGetLastError() != WSA_IO_PENDING) {
            return false;
        }
    }
    return true;
}

ESocketResult SocketIOCP::CheckTransmit(IN int completeByte)
{
    IOContext* txData = txQueue.Front();
    txData->current += completeByte;

    // Complete
    if(txData->current == txData->target) {
        txQueue.Deque();

        if(txQueue.IsEmpty() == false) {
            txData = txQueue.Front(); // Next
            if(false == TransmitAsync()) {
                return ESocketResult::SOC_ERROR; // Failed
            }
            return ESocketResult::SOC_CONTINUE; // Continue
        }
        return ESocketResult::SOC_TRUE; // Complete => Next
    }

    // Redo (continue)
    if(false == TransmitAsync()) {
        return ESocketResult::SOC_ERROR; // Failed
    }
    return ESocketResult::SOC_FALSE; // Not complete => Continue
}

ESocketResult SocketIOCP::CheckReceive(IN int completeByte)
{
    rxInfo.current += completeByte;

    // Get Size
    if(isGotSize == false) {
        // Complete => Prepare continue
        if(rxInfo.current == sizeof(PayloadSize)) {
            rxInfo.target  = *(int*)rxInfo.buffer;
            rxInfo.current = 0;
            isGotSize      = true;
        }

        // Redo (continue)
        if(false == ReceiveAsync()) {
            return ESocketResult::SOC_ERROR; // Failed
        }
        return ESocketResult::SOC_FALSE; // Not complete => Continue
    }

    // Else, complete
    if(rxInfo.current == rxInfo.target) {
        rxInfo.current = 0;
        rxInfo.target  = 0;
        isGotSize      = false;

        return ESocketResult::SOC_TRUE; // Complete
    }

    else {
        if(false == ReceiveAsync()) {
            return ESocketResult::SOC_ERROR; // Failed
        }
        return ESocketResult::SOC_FALSE; // Not complete => Continue
    }
}

void SocketIOCP::Packing(IN const Ptr payload, IN DataSize size, IN Protocol protocol)
{
    PayloadSize total = size + EPacketSize::INFO; // Send Size: include => (data,
                                                  // sequentil, protocol) size

    IOContext* txTemp = new IOContext(total + 1);
    Byte*      cursor = txTemp->buffer;

    *(PayloadSize*)cursor = total - sizeof(PayloadSize); // Set Payload size
    cursor += sizeof(DataSize);                          // Move

    memcpy(cursor, &index, sizeof(PacketIndex)); // Set Packet idex
    cursor += sizeof(PacketIndex);               // Move
    ++index;                                     // Next

    memcpy(cursor, &protocol, sizeof(Protocol)); // Set protocol
    cursor += sizeof(Protocol);                  // Move

    if(size > 0) {
        memcpy(cursor, payload, size); // Set data, Not move
    }

    txTemp->target  = total;
    txTemp->current = 0;
    txQueue.Enque(txTemp); // No copy (prevent buffer deletion) => Use pointer
}

// Parameter is nullptr => Ignore
// false: Packet number mismatch
bool SocketIOCP::Unpacking(OUT OPT Ptr buffer, OUT OPT Protocol* protocol, IN ssize_t bufferSize)
{
    Byte* cursor = reinterpret_cast<Byte*>(rxInfo.buffer);

    PacketIndex sequence = *reinterpret_cast<PacketIndex*>(rxInfo.buffer);
    if(sequence != sequence) {
        return false;
    }

    cursor += sizeof(PacketIndex);
    if(protocol != nullptr) {
        *protocol = *reinterpret_cast<Protocol*>(cursor);
    }

    cursor += sizeof(Protocol);
    if(static_cast<void*>(buffer) != nullptr) {
        memcpy(buffer, cursor, bufferSize);
    }
    return true;
}

// throw
// bad_alloc: new
// runtime_error: socket()
SocketIOCP* SocketIOCP::CreateServer(IN u_short port)
{
    SocketIOCP* ptr = new SocketIOCP();
    if(ptr == nullptr) {
        throw std::bad_alloc();
    }

    ptr->handle = socket(AF_INET, SOCK_STREAM, 0);
    if(ptr->handle == INVALID_SOCKET) {
        delete ptr;
        throw std::runtime_error(win::ErrorFormatter::Formatting(GetLastError()));
    }

    ptr->info.sin_family      = AF_INET;
    ptr->info.sin_addr.s_addr = htonl(INADDR_ANY);
    ptr->info.sin_port        = htons(port);

    return ptr;
}

// throw
// bad_alloc: new
SocketIOCP* SocketIOCP::CreateSession(IN SOCKET hSocket, IN const SOCKADDR_IN& sockAddr)
{
    SocketIOCP* ptr = new SocketIOCP();
    if(ptr == nullptr) {
        throw std::bad_alloc();
    }

    ptr->handle = hSocket;
    ptr->info   = sockAddr;

    return ptr;
}

bool SocketState::Receive()
{
    return parent->ReceiveAsync();
}

bool SocketState::Transmit()
{
    return parent->TransmitAsync();
}

SocketState::SocketState(IN SocketIOCP* parent): parent(parent) {}

SocketState::~SocketState()
{
    SAFE_DELETE(parent);
}

bool SocketState::Receive()
{
    return parent->ReceiveAsync();
}

bool SocketState::Transmit()
{
    return parent->TransmitAsync();
}

SocketController::SocketController(): state(nullptr) {}

SocketController::SocketController(SocketState* param): state(param) {}

SocketController::~SocketController()
{
    SAFE_DELETE(state);
}

bool SocketController::Receive()
{
    return state->Receive();
}
bool SocketController::Transmit()
{
    return state->Transmit();
}

SocketState::ENext SocketController::PostReceive()
{
    return state->PostReceive();
}

SocketState::ENext SocketController::PostTransmit()
{
    return state->PostTransmit();
}

SocketState* SocketController::GetNext()
{
    return state->NextState();
}

bool SocketController::IsSet()
{
    return state != nullptr;
}

void SocketController::SetState(IN SocketState* state)
{
    SAFE_DELETE(this->state);
    this->state = state;
}

void SocketController::ChangeState()
{
    if(!state) {
        return;
    }
    SocketState* temp;

    temp = state->NextState();
    delete state;
    state = temp;
}

} // namespace iocp
} // namespace sbl