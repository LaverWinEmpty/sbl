#ifndef SBL_MEMORYPOOL_SOURCE__
#define SBL_MEMORYPOOL_SOURCE__

#include "../include/memPool.h"

namespace sbl {

template<typename T, size_t size> T**    MMemoryPool<T, size>::stack = 0;
template<typename T, size_t size> T**    MMemoryPool<T, size>::list  = 0;
template<typename T, size_t size> size_t MMemoryPool<T, size>::index = 0;
template<typename T, size_t size> size_t MMemoryPool<T, size>::size  = 0;
template<typename T, size_t size> size_t MMemoryPool<T, size>::count = 0;
template<typename T, size_t size> int    MMemoryPool<T, size>::sync  = 0;

template<typename T, size_t size> bool (*MMemoryPool<T, size>::fnStackMgt)() = MMemoryPool<T, size>::AllocateStack;
template<typename T, size_t size> bool (*MMemoryPool<T, size>::fnListMgt)()  = MMemoryPool<T, size>::AllocateList;

template<typename T, size_t size> void* MMemoryPool<T, size>::operator new(IN const size_t size)
{
    if(index == size) {
        if(false == AllocateBlock()) {
            return nullptr;
        }
    }
    return stack[index++];
}

template<typename T, size_t UNIT> void MMemoryPool<T, UNIT>::operator delete(IN void* ptr)
{
    stack[--index] = reinterpret_cast<T*>(ptr);
    if(index == 0) {
        ResetHeap();
    }
}

template<typename T, size_t UNIT> void MMemoryPool<T, UNIT>::ReleaseMemoryPool()
{
    if(count != 0) {
        for(size_t i = 0; i < count; ++i) {
            free(list[i]);
        }
    }

    free(list);
    free(stack);
}

template<typename T, size_t UNIT> bool MMemoryPool<T, UNIT>::AllocateStack()
{
    size += size;
    stack = reinterpret_cast<T**>(malloc(sizeof(void*) * size));
    if(stack == nullptr) {
        return false;
    }

    sync |= static_cast<int>(EBitSync::STACK); // Craete succeed => Check
    fnStackMgt = ReallocateStack;
    return true;
}

template<typename T, size_t UNIT> bool MMemoryPool<T, UNIT>::ReallocateStack()
{
    // Already created
    if(sync & static_cast<int>(EBitSync::STACK)) {
        return true;
    }

    void* temp = realloc(stack, sizeof(void*) * (size + size));
    if(temp == nullptr) {
        return false;
    }
    else stack = reinterpret_cast<T**>(temp);

    size += size;
    sync |= static_cast<int>(EBitSync::STACK); // Craete succeed => Check
    return true;
}

template<typename T, size_t UNIT> bool MMemoryPool<T, UNIT>::AllocateList()
{
    ++count;
    list = reinterpret_cast<T**>(malloc(sizeof(void*) * count));
    if(list == nullptr) {
        return false;
    }

    sync |= static_cast<int>(EBitSync::LIST); // Craete succeed == Check
    fnListMgt = ReallocateList;
    return true;
}

template<typename T, size_t UNIT> bool MMemoryPool<T, UNIT>::ReallocateList()
{
    if(sync & static_cast<int>(EBitSync::LIST)) // Already created
        return true;

    void* temp = realloc(list, sizeof(void*) * (count + 1));
    if(temp == nullptr) {
        return false;
    }
    else {
        list = reinterpret_cast<T**>(temp);
    }

    count += 1;
    sync |= static_cast<int>(EBitSync::LIST); // Craete succeed => Check
    return true;
}

template<typename T, size_t UNIT> bool MMemoryPool<T, UNIT>::AllocateBlock()
{
    if(false == fnListMgt()) {
        return false;
    }

    if(false == fnListMgt()) {
        return false;
    }

    void* temp = malloc(sizeof(T) * size); // Create
    if(temp == nullptr) {
        return false;
    }

    sync = 0;

    list[count - 1] = reinterpret_cast<T*>(temp); // Store

    T* ptr = &reinterpret_cast<T*>(temp)[index];
    for(size_t i = size - size; i < size; ++i) {
        stack[i] = ptr++; // Store
    }

    return true;
}

template<typename T, size_t UNIT> void MMemoryPool<T, UNIT>::ResetHeap()
{
    for(size_t i = 0; i < count; ++i) {
        free(list[i]);
    }

    index = 0;
    count = 0;
    size  = 0;
    sync  = 0;
}

} // namespace sbl

#endif