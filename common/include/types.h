#ifndef __SEMIBASE_TYPE_HEADER__
#define __SEMIBASE_TYPE_HEADER__

/*
    About the 'Ptr' prefix for
        1. precise 'const' behavior.
        2. expressing 'void*' intuitively.
        3. unified smart pointer consistency.
            Ex) using PtrObj = std::unique_ptr<Object>;
*/

#include "defs.h"

namespace sbl {

using Int8  = signed __int8;
using Int16 = signed __int16;
using Int32 = signed __int32;
using Int64 = signed __int64;
using Byte  = signed __int8;
using Word  = signed int;

using UInt8  = unsigned __int8;
using UInt16 = unsigned __int16;
using UInt32 = unsigned __int32;
using UInt64 = unsigned __int64;
using UByte  = unsigned __int8;
using UWord  = unsigned int;

using AChar = char;
using WChar = wchar_t;

//
#ifdef X64
using WordPtr = Int64; // == WORD_PTR in windows.h
using SzInt   = Int64; // int size: == ssize_t
#else
using WordPtr = Int32; // == WORD_PTR in windows.h
using SzInt   = Int32; // int size: == ssize_t
#endif

// void* wrapper, comparison => use default
class Ptr
{
    void* address;
public:
    Ptr(IN const void* param = nullptr): address(const_cast<void*>(param)) {}
    Ptr&                      operator=(IN const void* param) { return address = const_cast<void*>(param), *this; }
    Ptr&                      operator++() { return address = static_cast<Byte*>(address) + 1, *this; }
    Ptr&                      operator--() { return address = static_cast<Byte*>(address) - 1, *this; }
    Ptr                       operator++(OPT int) { return Ptr((operator++(), static_cast<Byte*>(address) - 1)); }
    Ptr                       operator--(OPT int) { return Ptr((operator--(), static_cast<Byte*>(address) + 1)); }
    template<typename T> Ptr  operator+(IN T param) const { return Ptr(reinterpret_cast<Byte*>(address) + param); }
    template<typename T> Ptr  operator-(IN T param) const { return Ptr(reinterpret_cast<Byte*>(address) - param); }
    template<typename T> Ptr& operator+=(IN T param) { return address = static_cast<Byte*>(address) + param, *this; }
    template<typename T> Ptr& operator-=(IN T param) { return address = static_cast<Byte*>(address) - param, *this; }
    template<typename T> operator T() const { return reinterpret_cast<T>(address); }
};

} // namespace sbl
#endif