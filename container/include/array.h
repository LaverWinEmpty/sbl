#ifndef SEMIBASE_ARRAY_HEADER__
#define SEMIBASE_ARRAY_HEADER__

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
    void Reallocation(IN size_t dataSize);
    void IncreaseMemory(IN int incMem = sizeUnit);
    void DecreaseMemory(IN int decMem = sizeUnit);

public:
    Array();
    ~Array() { SAFE_DELETES(mem); }
    T& operator[](int index);

public:
    int GetSize() { return size; }
    int GetCapacity() { return capacity; }

public:
    prop(get = GetSize) int Size;
    prop(get = GetCapacity) int Capacity;

public:
    bool IsEmpty() { return size == 0; }

private:
    bool IsFull() { return size >= capacity; }
    bool IsValueDecreasing() { return size <= (capacity - (int)(sizeUnit + (sizeUnit >> 1))); }

public:
    void Push(IN const T& ref);
    bool Pop();
    bool Remove(IN int index);

private:
    SzInt size     = 0;
    int   capacity = 0;
    T*    mem      = nullptr;
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