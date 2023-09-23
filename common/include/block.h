#ifndef __SEMIBASE_BLOCK_STRUCTURE_HEADER__
#define __SEMIBASE_BLOCK_STRUCTURE_HEADER__

#include "types.h"
#include "error.h"

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
        => From: data[n] = static_cast<UByte>(arg), in Ctor(SzInt index, T arg, Args... args)
*/

#define DECLARE_BLOCK(bit)                                                                                             \
    struct Block##bit                                                                                                  \
    {                                                                                                                  \
        UByte& operator[](IN SzInt index)                                                                              \
        {                                                                                                              \
            return data[index];                                                                                        \
        }                                                                                                              \
        Block##bit(const AChar* str): data{ 0 }                                                                        \
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
        template<typename T, typename... Args> void Ctor(SzInt index, T arg, Args... args)                             \
        {                                                                                                              \
            data[index] = static_cast<T>(arg);                                                                         \
            if(index >= TO_BYTE(bit)) {                                                                                \
                throw ErrorBuilder::OutOfRange();                                                                     \
            }                                                                                                          \
            Ctor(index + 1, args...);                                                                                  \
        }                                                                                                              \
        void Ctor(SzInt index)                                                                                         \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
        UByte data[TO_BYTE(bit)];                                                                                      \
    }

namespace sbl {

    DECLARE_BLOCK(128);
    DECLARE_BLOCK(256);
    DECLARE_BLOCK(512);

} // namespace sbl
#undef DECLARE_BLOCK
#endif