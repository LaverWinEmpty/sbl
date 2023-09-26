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
#ifndef _WINDOWS_
    template<int N> struct Wrapper
    {
        std::mutex instance;

        Wrapper();
        ~Wrapper();
    };
    static Wrapper<KEY> wrapper;
#else
    template<int N> struct Wrapper
    {
        CRITICAL_SECTION instance;

        Wrapper();
        ~Wrapper();
    };
    static Wrapper<KEY> wrapper;
#endif

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
