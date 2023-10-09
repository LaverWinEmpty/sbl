#ifndef SBL_TYPE_HEADER__
#define SBL_TYPE_HEADER__

#include "defs.h"

// a parts of the standard
#ifndef _STDINT
#    if _MSC_VER
using int8_t   = signed __int8;
using int16_t  = signed __int16;
using int32_t  = signed __int32;
using int64_t  = signed __int64;
using uint8_t  = unsigned __int8;
using uint16_t = unsigned __int16;
using uint32_t = unsigned __int32;
using uint64_t = unsigned __int64;
#    else
using int8_t   = signed char;
using int16_t  = signed short;
using int32_t  = signed int;
using int64_t  = signed long long;
using uint8_t  = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;
#    endif
#endif

#ifndef _SSIZE_T_DEFINED_
#    define _SSIZE_T_DEFINED_
#    if defined(X64)
using ssize_t = int64_t;
#    elif defined(X86)
using ssize_t  = int32_t;
#    else
error Unknown OS
#    endif
#endif

/*
    Cunstructor
    1. String
        EX) "ABCD"
            => 0x41, 0x42, 0x43, 0x44, 0x00 ... 0x00
    2. Template
        It is for Variadic parameter, Not variadic template
        EX) { 1, 2, 3, 4 }
            => 0x01, 0x02, 0x03, 0x04, 0x00 ... 0x00

    Error
    1. Out of range
        => Parameter count exceeds size
    2. Static cast assert
        => From: data[n] = static_cast<UByte>(arg), in Ctor(ssize_t index, T arg, Args... args)
*/

#define DECLARE_BLOCK(bit)                                                                                             \
    struct Block##bit                                                                                                  \
    {                                                                                                                  \
        UByte& operator[](IN ssize_t index)                                                                            \
        {                                                                                                              \
            return data[index];                                                                                        \
        }                                                                                                              \
        Block##bit(const Byte* str): data{ 0 }                                                                         \
        {                                                                                                              \
            for(int i = 0; str[i] || i < TO_BYTE(bit); ++i) {                                                          \
                data[i] = str[i];                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
        template<typename... Args> Block##bit(Args... args): data{ 0 }                                                 \
        {                                                                                                              \
            Ctor(0, args...);                                                                                          \
        }                                                                                                              \
        template<typename T> operator T()                                                                              \
        {                                                                                                              \
            return reinterpret_cast<T>(data);                                                                          \
        }                                                                                                              \
    private:                                                                                                           \
        template<typename T, typename... Args> void Ctor(ssize_t index, T arg, Args... args)                           \
        {                                                                                                              \
            data[index] = static_cast<T>(arg);                                                                         \
            if(index >= TO_BYTE(bit)) {                                                                                \
                throw std::out_of_range(EErrMsg::OUT_OF_RANGE);                                                        \
            }                                                                                                          \
            Ctor(index + 1, args...);                                                                                  \
        }                                                                                                              \
        void Ctor(ssize_t index)                                                                                       \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
        UByte data[TO_BYTE(bit)];                                                                                      \
    }

namespace sbl {

using Wrod  = signed int;
using UWord = unsigned int;

using Byte  = int8_t;
using UByte = uint8_t;

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

DECLARE_BLOCK(128);
DECLARE_BLOCK(256);
DECLARE_BLOCK(512);

} // namespace sbl
#endif