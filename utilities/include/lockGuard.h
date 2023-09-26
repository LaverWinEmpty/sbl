#ifndef SBL_WIN_LOCKGUARD_HEADER__
#define SBL_WIN_LOCKGUARD_HEADER__

// Check windows.h (=> for using CRITICAL_SECTION)
#ifndef _WINDOWS_
#    include "mutex"
#endif

namespace sbl {

template<int KEY> class LockGuard
{
private:
    template<int N> struct Wrapper
    {
#ifdef _WINDOWS_
        CRITICAL_SECTION instance;
#else
        std::mutex instance;
#endif

        Wrapper();
        ~Wrapper();
    };
    static Wrapper<KEY> wrapper;

private:
    LockGuard(int); // Unlocked LockGuard

public:
    LockGuard();
    ~LockGuard();

public:
    void Lock();
    void Unlock();

public:
    static LockGuard Locked();
    static LockGuard Unlocked();
};

} // namespace sbl
#endif
