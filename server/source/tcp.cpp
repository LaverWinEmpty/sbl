#include "../include/tcp.h"

namespace sbl {
namespace iocp {

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

DWORD SocketTCP::TransmitSync(IN Byte* buffer, IN SzInt size, IN Protocol protocol)
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

DWORD SocketTCP::ReceiveSync(OUT OPT Byte* buffer, OUT OPT Protocol* protocol, OUT OPT DataSize* received)
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

void SocketTCP::NextIndex()
{
    ++index;
}
SOCKET SocketTCP::GetSocketHandle() const
{
    return handle;
}
PacketIndex SocketTCP::GetPacketNumber() const
{
    return index;
}

} // namespace iocp
} // namespace sbl