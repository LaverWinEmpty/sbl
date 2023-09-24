#ifndef SBL_MEMORYPOOL_HEADER__
#define SBL_MEMORYPOOL_HEADER__

#include "../common/include/common.h"

namespace sbl {

template<typename T, size_t UNIT = 256> class MMemoryPool abstract
{
private:
    enum class EBitSync
    {
        STACK = 1 << 0,
        LIST  = 1 << 1
    };

public:
    static void* operator new(IN size_t size);
    static void  operator delete(IN void* ptr);

public:
    static void ReleaseMemoryPool();

private:
    static bool AllocateStack();
    static bool ReallocateStack();
    static bool AllocateList();
    static bool ReallocateList();
    static bool AllocateBlock();
    static void ResetHeap();

private:
    static T** stack; // Pointer list => Stack
    static T** list;  // Pointer list => Block start address list

private:
    static size_t index; // Stack cursor
    static size_t size;  // Stack size
    static size_t count; // Allocate count => Block count / Block list size

private:
    static int sync; // Stack / List / Instance Heap Check

private:
    static bool (*fnStackMgt)(); // Method
    static bool (*fnListMgt)();  // Method
};

} // namespace sbl
#endif