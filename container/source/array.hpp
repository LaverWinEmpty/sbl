#ifndef __SEMIBASE_ARRAY_SOURCE__
#define __SEMIBASE_ARRAY_SOURCE__

#include "../include/array.h"

namespace sbl {

// Throw ErrMsg
template<typename T, UWord n> void Array<T, n>::Reallocation(IN size_t dataSize)
{
    if(dataSize == capacity) {
        return;
    }

    T* newMem = new T[capacity];
    if(newMem == nullptr) {
        throw SErrorBuilder::NewFailed();
    }

    // Move
    for(size_t i = 0; i < dataSize; ++i) {
        newMem[i] = mem[i];
    }

    SAFE_DELETES(mem);
    mem = newMem;
}

template<typename T, UWord n> void Array<T, n>::IncreaseMemory(IN int incMem)
{
    if(incMem < 0) {
        return;
    }

    int oldSize = capacity;
    capacity += incMem;
    Reallocation(oldSize);
}

template<typename T, UWord n> void Array<T, n>::DecreaseMemory(IN int decMem)
{
    if(decMem < 0) {
        return;
    }

    int newSize = capacity - decMem;
    capacity -= decMem;
    Reallocation(newSize);
}

template<typename T, UWord n> Array<T, n>::Array()
{
    IncreaseMemory();
}

template<typename T, UWord n> T& Array<T, n>::operator[](IN int index)
{
    if(index < 0) {
        return mem[0];
    }

    if(index >= capacity) {
        int incMem = index - size;
        incMem     = (incMem / n + 1) * n; // Round up
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

template<typename T, UWord n> bool Array<T, n>::Remove(IN int index)
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