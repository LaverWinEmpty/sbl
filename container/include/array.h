#ifndef SBL_ARRAY_HEADER__
#define SBL_ARRAY_HEADER__

#include "../../common/include/common.h"

/*
    Array
        - Free index access: When an index is exceeded, it is newly allocated.

    Queue
        - Array wrapper
        - Inline method only, but using Array. So need this file.
        - Simple Queue
            - ADT: Enque, Deque, Front, Size, IsEmpty
*/

namespace sbl {

template<typename T, UWord sizeUnit = 10> class Array
{
private:
    void Reallocation(IN ssize_t dataSize);
    void IncreaseMemory(IN ssize_t incMem = sizeUnit);
    void DecreaseMemory(IN ssize_t decMem = sizeUnit);

public:
    Array();
    ~Array();
    T& operator[](ssize_t index);

public:
    ssize_t GetSize();
    ssize_t GetCapacity();
    bool    IsEmpty();

private:
    bool IsFull();
    bool IsValueDecreasing();

public:
    void Push(IN const T& ref);
    bool Pop();
    bool Remove(IN ssize_t index);

public:
    prop(get = GetSize) ssize_t Size;
    prop(get = GetCapacity) ssize_t Capacity;

private:
    ssize_t size     = 0;
    ssize_t capacity = 0;
    T*      mem      = nullptr;
};

template<typename T, UWord sizeUnit = 10> class Queue
{
public:
    T&   Front() { return array[0]; }
    void Enque(IN const T& ref) { array.Push(ref); }
    void Deque() { array.Remove(0); }
    bool IsEmpty() { return array.IsEmpty(); }
    int  GetSize() { return array.Size; }

private:
    Array<T> array;
};

template<typename T, UWord sizeUnit = 10> class Stack
{
public:
    T&   Top() { return array[0]; }
    void Push(IN const T& ref) { array.Push(ref); }
    void Pop() { array.Pop(); }
    bool IsEmpty() { return array.IsEmpty(); }
    int  GetSize() { return array.Size; }

private:
    Array<T> array;
};

} // namespace sbl
#endif