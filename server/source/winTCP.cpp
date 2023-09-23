#include "../include/winTCP.h"

namespace sbl {
namespace iocp {

SocketTCP::SocketTCP()
{
    rxOperate.parent = this;
    txOperate.parent = this;
    txOperate.type   = AsyncOperation::IO_WRITE;
    rxOperate.type   = AsyncOperation::IO_READ;
}

// Failed: return INVALID_SOCKET
SOCKET SocketTCP::Accept(OUT OPT SOCKADDR_IN* info)
{
    int         addrLen = TO_INT(sizeof(SOCKADDR_IN));
    SOCKET      sock;
    SOCKADDR_IN addr;

    sock = accept(handle, reinterpret_cast<SOCKADDR*>(&addr), &addrLen); // Not check
    if(info != nullptr) {
        *info = addr; // Out
    }
    return sock;
}

// Succeed: return 0
DWORD SocketTCP::Connect()
{
    if(SOCKET_ERROR == connect(handle, (SOCKADDR*)&info, sizeof(info))) {
        return GetLastError();
    }
    return 0;
}

// Succeed: return 0
DWORD SocketTCP::Bind()
{
    if(SOCKET_ERROR == bind(handle, (SOCKADDR*)&info, sizeof(info))) {
        return GetLastError();
    }
    return 0;
}

// Succeed: return 0
DWORD SocketTCP::Listen()
{
    if(SOCKET_ERROR == listen(handle, SOMAXCONN)) {
        return GetLastError();
    }
    return 0;
}

DWORD SocketTCP::Transmit(IN Byte* buffer, IN SzInt size, IN Protocol protocol)
{
    // Send Size: include => (data, sequentil, protocol) size
    PayloadSize total = size + EPacketSize::INFO;

    Byte* packet = new Byte[total];
    if(packet == nullptr) {
        return GetLastError();
    }

    Byte* cursor = packet;

    *(PayloadSize*)cursor = total - sizeof(PayloadSize); // Set DATA size
    cursor += sizeof(PayloadSize);                       // Move

    *(PacketIndex*)cursor = index; // Set sequential
    cursor += sizeof(PacketIndex); // Move

    *(Protocol*)cursor = protocol; // Set protocol
    cursor += sizeof(Protocol);    // Move

    if(size > 0) {
        memcpy(cursor, buffer, size); // Set data, Not move
    }

    int res = 0;
    if(SOCKET_ERROR == send(handle, reinterpret_cast<char*>(packet), static_cast<int>(total), NULL)) {
        res = GetLastError();
    }

    delete[] packet;
    return res;
}

DWORD SocketTCP::Receive(OUT OPT Byte* buffer, OUT OPT Protocol* protocol, OUT OPT DataSize* received)
{
    PayloadSize size;
    int         result;

    result = recv(handle, (char*)&size, sizeof(PayloadSize), NULL);
    if(result <= 0) {
        return (int)GetLastError();
    }

    char temp[EPacketSize::PACKET];
    result = recv(handle, temp, size, NULL);
    if(result <= 0) {
        return (int)GetLastError();
    }

    char* cursor = temp;

    PacketIndex packetNumber = *(PacketIndex*)cursor;
    if(packetNumber != index) {
        return SOCKET_ERROR;
    }
    cursor += sizeof(PacketIndex);

    if(protocol != nullptr) {
        *protocol = *(Protocol*)cursor;
    }
    cursor += sizeof(Protocol);

    if(buffer != nullptr) {
        memcpy(buffer, cursor, size - sizeof(PacketIndex) - sizeof(Protocol));
    }

    if(received != nullptr) {
        *received = result;
    }
    return 0;
}

bool SocketTCP::Transmit()
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

bool SocketTCP::Receive()
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

ESocketResult SocketTCP::CheckTransmit(IN int completeByte)
{
    IOContext* txData = txQueue.Front();
    txData->current += completeByte;

    // Complete
    if(txData->current == txData->target) {
        txQueue.Deque();

        if(txQueue.IsEmpty() == false) {
            txData = txQueue.Front(); // Next
            if(false == Transmit()) {
                return ESocketResult::SOC_ERROR; // Failed
            }
            return ESocketResult::SOC_CONTINUE; // Continue
        }
        return ESocketResult::SOC_TRUE; // Complete => Next
    }

    // Redo (continue)
    if(false == Transmit()) {
        return ESocketResult::SOC_ERROR; // Failed
    }
    return ESocketResult::SOC_FALSE; // Not complete => Continue
}

ESocketResult SocketTCP::CheckReceive(IN int completeByte)
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
        if(false == Receive()) {
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
        if(false == Receive()) {
            return ESocketResult::SOC_ERROR; // Failed
        }
        return ESocketResult::SOC_FALSE; // Not complete => Continue
    }
}

void SocketTCP::Packing(IN const Ptr payload, IN DataSize size, IN Protocol protocol)
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
bool SocketTCP::Unpacking(OUT OPT Ptr buffer, OUT OPT Protocol* protocol, IN SzInt bufferSize)
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

SocketTCP* SocketBuilder::Server(IN u_short port)
{
    SocketTCP* ptr = new SocketTCP();
    if(ptr == nullptr) {
        throw ErrorBuilder::NewFailed();
    }

    ptr->handle = socket(AF_INET, SOCK_STREAM, 0);
    if(ptr->handle == INVALID_SOCKET) {
        delete ptr;
        throw ErrorBuilder::CreateFailed();
    }

    ptr->info.sin_family      = AF_INET;
    ptr->info.sin_addr.s_addr = htonl(INADDR_ANY);
    ptr->info.sin_port        = htons(port);

    return ptr;
}

SocketTCP* SocketBuilder::Client(IN const AChar* ip, IN u_short port)
{
    SocketTCP* ptr = new SocketTCP();
    if(ptr == nullptr) {
        throw ErrorBuilder::NewFailed();
    }

    ptr->handle = socket(AF_INET, SOCK_STREAM, 0);
    if(ptr->handle == INVALID_SOCKET) {
        delete ptr;
        throw ErrorBuilder::CreateFailed();
    }

    ptr->info.sin_family      = AF_INET;
    ptr->info.sin_addr.s_addr = inet_addr(ip);
    ptr->info.sin_port        = htons(port);

    return ptr;
}

SocketTCP* SocketBuilder::Session(IN SOCKET hSocket, IN const SOCKADDR_IN& sockAddr)
{
    SocketTCP* ptr = new SocketTCP();
    if(ptr == nullptr) {
        throw ErrorBuilder::NewFailed();
    }

    ptr->handle = hSocket;
    ptr->info   = sockAddr;

    return ptr;
}

bool SocketState::Receive()
{
    return parent->Receive();
}

bool SocketState::Transmit()
{
    return parent->Transmit();
}

SocketState::SocketState(IN SocketTCP* parent): parent(parent) {}

SocketState::~SocketState()
{
    SAFE_DELETE(parent);
}

bool SocketState::Receive()
{
    return parent->Receive();
}

bool SocketState::Transmit()
{
    return parent->Transmit();
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