#ifndef __SEMIBASE_UNSIGEND_INT_128_HEADER__
#define __SEMIBASE_UNSIGEND_INT_128_HEADER__

// Bit operator only

#include "types.h"

namespace sbl {

struct UInt128
{
    UInt64 high, low;

    UInt128();
    UInt128(IN const UInt128& ref);
    UInt128(IN UInt64 high, IN UInt64 low);
    UInt128(IN UInt64 ll);
    UInt128& operator=(IN const UInt128& ref);
    bool     operator==(IN const UInt128& ref) const;
    bool     operator!=(IN const UInt128& ref) const;
    bool     operator>(IN const UInt128& ref) const;
    bool     operator<(IN const UInt128& ref) const;
    bool     operator>=(IN const UInt128& ref) const;
    bool     operator<=(IN const UInt128& ref) const;
    bool     operator!() const;
    UInt128  operator^(IN const UInt128&) const;
    UInt128  operator&(IN const UInt128&) const;
    UInt128  operator|(IN const UInt128&) const;
    UInt128  operator<<(IN size_t) const;
    UInt128  operator>>(IN size_t) const;
    UInt128  operator~() const;
    UInt128& operator^=(IN const UInt128&);
    UInt128& operator&=(IN const UInt128&);
    UInt128& operator|=(IN const UInt128&);
    UInt128& operator<<=(IN size_t);
    UInt128& operator>>=(IN size_t);
    template<typename T> explicit operator T() const;
};

inline UInt128::UInt128(): high(0), low(0) {}

inline UInt128::UInt128(IN const UInt128& ref): high(ref.high), low(ref.low) {}

inline UInt128::UInt128(IN UInt64 high, IN UInt64 low): high(high), low(low) {}

inline UInt128::UInt128(IN UInt64 ll): high(0), low(ll)
{
    if(ll & 0x8000000000000000) {
        high = -1;
    }
}

inline UInt128& UInt128::operator=(IN const UInt128& ref)
{
    high = ref.high;
    low  = ref.low;
    return *this;
}

inline bool UInt128::operator==(IN const UInt128& ref) const
{
    return high == ref.high && low == ref.low;
}

inline bool UInt128::operator!=(IN const UInt128& ref) const
{
    return high != ref.high || low != ref.low;
}

inline bool UInt128::operator>(IN const UInt128& ref) const
{
    return high > ref.high || (high == ref.high && low > ref.low);
}

inline bool UInt128::operator<(IN const UInt128& ref) const
{
    return high < ref.high || (high == ref.high && low < ref.low);
}

inline bool UInt128::operator>=(IN const UInt128& ref) const
{
    return high >= ref.high && low >= ref.low;
}

inline bool UInt128::operator<=(IN const UInt128& ref) const
{
    return high <= ref.high && low <= ref.low;
}

inline bool UInt128::operator!() const
{
    return high == 0 && low == 0;
}

inline UInt128 UInt128::operator^(IN const UInt128& ref) const
{
    return { high ^ ref.high, low ^ ref.low };
}

inline UInt128 UInt128::operator&(IN const UInt128& ref) const
{
    return { high & ref.high, low & ref.low };
}

inline UInt128 UInt128::operator|(IN const UInt128& ref) const
{
    return { high | ref.high, low | ref.low };
}

inline UInt128 UInt128::operator<<(IN size_t shift) const
{
    UInt128 temp;
    if(shift < SIZE_BIT(Int64)) {
        temp.high = high << shift;
        temp.low  = low << shift;
        temp.low |= low >> (SIZE_BIT(Int64) - shift);
    }
    else if(shift < SIZE_BIT(UInt128)) {
        temp.high = low << (shift - SIZE_BIT(Int64));
    }
    return temp;
}

inline UInt128 UInt128::operator>>(IN size_t shift) const
{
    UInt128 temp;
    if(shift < SIZE_BIT(Int64)) {
        temp.high = high >> shift;
        temp.low  = low >> shift;
        temp.low |= high << (SIZE_BIT(Int64) - shift);
    }
    else if(shift < SIZE_BIT(UInt128)) {
        temp.low = high >> (shift - SIZE_BIT(Int64));
    }
    return temp;
}

inline UInt128 UInt128::operator~() const
{
    return { ~high, ~low };
}

inline UInt128& UInt128::operator^=(IN const UInt128& ref)
{
    *this = *this ^ ref;
    return *this;
}

inline UInt128& UInt128::operator&=(IN const UInt128& ref)
{
    *this = *this & ref;
    return *this;
}

inline UInt128& UInt128::operator|=(IN const UInt128& ref)
{
    *this = *this | ref;
    return *this;
}

inline UInt128& UInt128::operator<<=(IN size_t shift)
{
    *this = *this << shift;
    return *this;
}

inline UInt128& UInt128::operator>>=(IN size_t shift)
{
    *this = *this >> shift;
    return *this;
}

template<typename T> inline UInt128::operator T() const
{
    return static_cast<T>(low);
}

} // namespace sbl
#endif