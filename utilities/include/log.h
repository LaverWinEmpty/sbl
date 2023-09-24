#ifndef SBL_LOG_HEADER__
#define SBL_LOG_HEADER__

#include "iostream"
#include "fstream"
#include "../../common/include/common.h"

namespace sbl {

interface ILoggable abstract
{
protected:
    virtual void OnWrite(IN std::ostream&) {}
    virtual void OnWrite(IN std::ofstream&) {}
    virtual void OnWrite(IN std::wostream&) {}
    virtual void OnWrite(IN std::wofstream&) {}

public:
    friend std::ostream&   operator<<(IN std::ostream& os, IN ILoggable& ref);
    friend std::ofstream&  operator<<(IN std::ofstream& os, IN ILoggable& ref);
    friend std::wostream&  operator<<(IN std::wostream& os, IN ILoggable& ref);
    friend std::wofstream& operator<<(IN std::wofstream& os, IN ILoggable& ref);
};

class Logger
{
public:
    DECLARE_LIMIT_LIFECYCLE(Logger);

public:
    static void SetLocale(IN const std::string& locale) { setlocale(LC_ALL, locale.c_str()); }
    static void SetLocale(IN const std::wstring& locale) { _wsetlocale(LC_ALL, locale.c_str()); }
    static void SetLocale(IN std::wofstream& fout, IN const std::string& locale) { fout.imbue(std::locale(locale)); }

public:
    template<typename T> static void WriteConsoleA(IN const std::string& delimiter, IN T arg);
    template<typename T, typename... Types>
    static void WriteConsoleA(IN const std::string& delimiter, IN T arg, IN Types... args);

public:
    template<typename T> static void WriteFileA(IN std::ofstream& fout, IN const std::string& delimiter, T arg);
    template<typename T, typename... Types>
    static void WriteFileA(IN std::ofstream& fout, IN const std::string& delimiter, IN T arg, IN Types... args);

public:
    template<typename T> static void WriteConsoleW(IN const std::wstring& delimiter, IN T arg);
    template<typename T, typename... Types>
    static void WriteConsoleW(IN const std::wstring& delimiter, IN T arg, IN Types... args);

public:
    template<typename T> static void WriteFileW(IN std::wofstream& fout, IN const std::wstring& delimiter, T arg);
    template<typename T, typename... Types>
    static void WriteFileW(IN std::wofstream& fout, IN const std::wstring& delimiter, IN T arg, IN Types... args);
};

} // namespace sbl
#endif