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
    void Reallocation(IN SzInt dataSize);
    void IncreaseMemory(IN SzInt incMem = sizeUnit);
    void DecreaseMemory(IN SzInt decMem = sizeUnit);

public:
    Array();
    ~Array();
    T& operator[](SzInt index);

public:
    SzInt GetSize();
    SzInt GetCapacity();
    bool  IsEmpty();

private:
    bool IsFull();
    bool IsValueDecreasing();

public:
    void Push(IN const T& ref);
    bool Pop();
    bool Remove(IN SzInt index);

public:
    prop(get = GetSize) SzInt Size;
    prop(get = GetCapacity) SzInt Capacity;

private:
    SzInt size     = 0;
    SzInt capacity = 0;
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