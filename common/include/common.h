#ifndef __SEMIBASE_COMMON_HEADER__
#define __SEMIBASE_COMMON_HEADER__

#include "defs.h"
#include "types.h"
#include "symbols.h"
#include "error.h"

namespace sbl {

template<typename T, typename U> struct Pair
{
    T key;
    U value;

    Pair operator=(IN const U& param) { return value = param, *this; }
    bool operator==(IN const Pair<T, U>& ref) const { return key == ref.key; }
    bool operator!=(IN const Pair<T, U>& ref) const { return key != ref.key; }
    bool operator<=(IN const Pair<T, U>& ref) const { return key <= ref.key; }
    bool operator>=(IN const Pair<T, U>& ref) const { return key >= ref.key; }
    bool operator<(IN const Pair<T, U>& ref) const { return key < ref.key; }
    bool operator>(IN const Pair<T, U>& ref) const { return key > ref.key; }

    operator U() { return value; }
};

template<typename T> struct Result abstract
{
    T* data;

    Result(IN Result&& ref) noexcept { data = ref.data, ref.data = nullptr; }
    Result(IN T* heap) noexcept { data = heap; }
    Result& operator=(IN Result& ref) = delete;
    T&      operator*() { return *data; }
    T*      operator->() { return data; }

    struct Value: public Result<T>
    {
        Value(IN T* heap): Result(heap) {}
        Value(IN Value&& ref): Result(Value && ref) {}
        ~Value() { SAFE_DESTRUCT(this->data); }
    };

    struct Array: public Result<T>
    {
        Array(IN T* heap): Result(heap) {}
        Array(IN Array&& ref): Result(Array && ref) {}
        ~Array() { SAFE_DESTRUCTS(this->data); }
        T& operator[](size_t index) { return this->data[index]; }
    };
};

template<typename T> class VTableInspector
{
    DECLARE_LIMIT_LIFECYCLE(VTableInspector);
    template<typename U> static __int32 Test(typeof(dynamic_cast<void*>(static_cast<U*>(nullptr)))) {}
    template<typename U> static __int64 Test(...) {}

    readonly bool HAS_VTABLE = sizeof(Test<T>(nullptr)) == sizeof(__int32);

public:
    static bool   HasVTable() { return VTableInspector<T>::HAS_VTABLE; }
    static Ptr    SkipVTable(const Ptr ptr) { return HAS_VTABLE ? Ptr(static_cast<Byte*>(ptr) + sizeof(Ptr)) : ptr; }
    static size_t SizeWithoutVTable() { return HAS_VTABLE ? sizeof(T) - sizeof(Ptr) : sizeof(T); }
};

} // namespace sbl
#endif
