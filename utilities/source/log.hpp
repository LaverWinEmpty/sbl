#ifndef __SEMIBASE_LOG_SOURCE__
#define __SEMIBASE_LOG_SOURCE__

#include "../include/log.h"

namespace sbl {

std::ostream& operator<<(IN std::ostream& os, IN ILoggable& ref)
{
    ref.OnWrite(os);
    return os;
}

std::ofstream& operator<<(IN std::ofstream& os, IN ILoggable& ref)
{
    ref.OnWrite(os);
    return os;
}

std::wostream& operator<<(IN std::wostream& os, IN ILoggable& ref)
{
    ref.OnWrite(os);
    return os;
}

std::wofstream& operator<<(IN std::wofstream& os, IN ILoggable& ref)
{
    ref.OnWrite(os);
    return os;
}

template<typename T> void Logger::WriteConsoleA(IN const std::string& delimiter, IN T arg)
{
    std::cout << arg << std::endl;
}

template<typename T, typename... Types>
void Logger::WriteConsoleA(IN const std::string& delimiter, IN T arg, IN Types... args)
{
    if(delimiter != nullptr) std::cout << arg << delimiter;
    else std::cout << arg;
    WriteConsoleA(delimiter, args...);
}

template<typename T> void Logger::WriteFileA(IN std::ofstream& fout, IN const std::string& delimiter, T arg)
{
    fout << arg << std::endl;
}

template<typename T, typename... Types>
void Logger::WriteFileA(IN std::ofstream& fout, IN const std::string& delimiter, IN T arg, IN Types... args)
{
    if(delimiter != nullptr) fout << arg << delimiter;
    else fout << arg;
    WriteFileA(fout, delimiter, args...);
}

template<typename T> void Logger::WriteConsoleW(IN const std::wstring& delimiter, IN T arg)
{
    std::wcout << arg << std::endl;
}

template<typename T, typename... Types>
void Logger::WriteConsoleW(IN const std::wstring& delimiter, IN T arg, IN Types... args)
{
    if(delimiter != nullptr) std::wcout << arg << delimiter;
    else std::wcout << arg;
    WriteConsoleW(delimiter, args...);
}

template<typename T> void Logger::WriteFileW(IN std::wofstream& fout, IN const std::wstring& delimiter, T arg)
{
    fout << arg << std::endl;
}

template<typename T, typename... Types>
void Logger::WriteFileW(IN std::wofstream& fout, IN const std::wstring& delimiter, IN T arg, IN Types... args)
{
    if(delimiter != nullptr) fout << arg << delimiter;
    else fout << arg;
    WriteFileW(fout, delimiter, args...);
}

} // namespace sbl
#endif