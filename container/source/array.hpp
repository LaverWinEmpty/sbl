#ifndef SBL_ARRAY_SOURCE__
#define SBL_ARRAY_SOURCE__

#include "../include/array.h"

namespace sbl {

template<typename T, UWord n> Array<T, n>::~Array()
{
    SAFE_DELETES(mem);
}

template<typename T, UWord n> SzInt Array<T, n>::GetSize()
{
    return size;
}

template<typename T, UWord n> SzInt Array<T, n>::GetCapacity()
{
    return capacity;
}

template<typename T, UWord n> bool Array<T, n>::IsEmpty()
{
    return size == 0;
}

template<typename T, UWord n> bool Array<T, n>::IsFull()
{
    return size >= capacity;
}

template<typename T, UWord n> bool Array<T, n>::IsValueDecreasing()
{
    return size <= (capacity - (int)(n + (n >> 1)));
}

// Throw ErrMsg
template<typename T, UWord n> void Array<T, n>::Reallocation(IN SzInt dataSize)
{
    if(dataSize == capacity) {
        return;
    }

    T* newMem = new T[capacity];
    if(newMem == nullptr) {
        throw ErrorBuilder::NewFailed();
    }

    // Move
    for(SzInt i = 0; i < dataSize; ++i) {
        newMem[i] = mem[i];
    }

    SAFE_DELETES(mem);
    mem = newMem;
}

template<typename T, UWord n> void Array<T, n>::IncreaseMemory(IN SzInt incMem)
{
    if(incMem < 0) {
        return;
    }

    SzInt oldSize = capacity;
    capacity += incMem;
    Reallocation(oldSize);
}

template<typename T, UWord n> void Array<T, n>::DecreaseMemory(IN SzInt decMem)
{
    if(decMem < 0) {
        return;
    }

    SzInt newSize = capacity - decMem;
    capacity -= decMem;
    Reallocation(newSize);
}

template<typename T, UWord n> Array<T, n>::Array()
{
    IncreaseMemory();
}

template<typename T, UWord n> T& Array<T, n>::operator[](IN SzInt index)
{
    if(index < 0) {
        return mem[0];
    }

    if(index >= capacity) {
        SzInt incMem = index - size;
        incMem       = (incMem / n + 1) * n; // Round up
        IncreaseMemory(incMem);
        size = index;
    }

    return mem[index];
}

template<typename T, UWord n> void Array<T, n>::Push(IN const T& ref)
{
    if(IsFull() == true) {
        IncreaseMemory();
    }
    mem[size++] = ref;
}

template<typename T, UWord n> bool Array<T, n>::Pop()
{
    if(IsEmpty() == true) {
        return false;
    }

    --size;
    if(IsValueDecreasing() == true) {
        DecreaseMemory();
    }

    return true;
}

template<typename T, UWord n> bool Array<T, n>::Remove(IN SzInt index)
{
    if(index < 0 || index >= capacity) {
        return false;
    }

    --size;
    int i    = index;
    int loop = size - index;
    FAST_LOOP(loop, mem[i] = mem[i + 1]; ++i;);

    if(IsValueDecreasing() == true) {
        DecreaseMemory();
    }

    return true;
}

} // namespace sbl
#endif