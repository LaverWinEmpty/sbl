#include "win.h"

std::string sbl::win::ErrorFormatter::Formatting(IN DWORD errorCode)
{
    LPVOID msg;
    DWORD  msgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                                  NULL,
                                  errorCode,
                                  0,
                                  reinterpret_cast<LPSTR>(&msg),
                                  0,
                                  NULL);

    char buffer[DEF_BUF_SIZE] = { 0 };
    if(msgLen) {
        sprintf_s(buffer, format, reinterpret_cast<char*>(msg), errorCode);
    }
    else {
        sprintf_s(buffer, format, EErrMsg::UNKNOWN, errorCode);
    }

    LocalFree(msg);
    return buffer;
}