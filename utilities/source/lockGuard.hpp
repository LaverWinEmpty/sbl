#ifndef SBL_WIN_LOCKGUARD_SOURCE__
#define SBL_WIN_LOCKGUARD_SOURCE__

#include "../include/lockGuard.h"

namespace sbl {

template<int KEY> LockGuard<KEY>::Wrapper<KEY> LockGuard<KEY>::wrapper;

template<int KEY> template<int N> LockGuard<KEY>::Wrapper<N>::Wrapper()
{
#ifdef _WINDOWS_
    InitializeCriticalSection(&instance);
#endif
}

template<int KEY> template<int N> LockGuard<KEY>::Wrapper<N>::~Wrapper()
{
#ifdef _WINDOWS_
    DeleteCriticalSection(&instance);
#endif
}

template<int KEY> LockGuard<KEY>::LockGuard(int) {}

template<int KEY> LockGuard<KEY>::LockGuard()
{
#ifdef _WINDOWS_
    EnterCriticalSection(&LockGuard<KEY>::wrapper.instance);
#else
    wrapper.instance.lock();
#endif
}

template<int KEY> LockGuard<KEY>::~LockGuard()
{
#ifdef _WINDOWS_
    LeaveCriticalSection(&LockGuard<KEY>::wrapper.instance);
#else
    wrapper.instance.unlock();
#endif
}

template<int KEY> void LockGuard<KEY>::Lock()
{
#ifdef _WINDOWS_
    EnterCriticalSection(&LockGuard<KEY>::wrapper.instance);
#else
    wrapper.instance.lock();
#endif
}

template<int KEY> void LockGuard<KEY>::Unlock()
{
#ifdef _WINDOWS_
    LeaveCriticalSection(&LockGuard<KEY>::wrapper.instance);
#else
    wrapper.instance.unlock();
#endif
}

template<int KEY> LockGuard<KEY> LockGuard<KEY>::Locked()
{
    return LockGuard<KEY>();
}

template<int KEY> LockGuard<KEY> LockGuard<KEY>::Unlocked()
{
    return LockGuard<KEY>(0);
}

} // namespace sbl
#endif