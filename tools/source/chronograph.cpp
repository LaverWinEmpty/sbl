#include "../include/chronograph.h"

namespace sbl {

template<typename T> const std::chrono::steady_clock::time_point Chronograph<T>::run = std::chrono::steady_clock::now();

template<typename T>
Chronograph<T>::Chronograph(): start(std::chrono::steady_clock::now()), last(std::chrono::steady_clock::now()), delta(0)
{}

template<typename T> void Chronograph<T>::Reset()
{
    start = std::chrono::steady_clock::now();
}

// Process: ns
// Return: sec
template<typename T> T Chronograph<T>::Stop() const
{
    std::chrono::duration<T> temp =
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start);
    return static_cast<T>(temp.count());
}

// First call: Initialize
template<typename T> void Chronograph<T>::UpdateDelta()
{
    std::chrono::high_resolution_clock::time_point curr;

    curr  = std::chrono::high_resolution_clock::now();
    delta = curr - last;
    last  = curr;
}

template<typename T> T Chronograph<T>::GetDeltaTimeMS() const
{
    return static_cast<T>(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count());
}

template<typename T> T Chronograph<T>::GetDeltaTimeUS() const
{
    return static_cast<T>(std::chrono::duration_cast<std::chrono::microseconds>(delta).count());
}

template<typename T> T Chronograph<T>::GetDeltaTimeNS() const
{
    return static_cast<T>(delta.count());
}

template<typename T> T Chronograph<T>::GetRunningTime()
{
    return static_cast<T>(
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - run).count());
}

template<typename T> T Chronograph<T>::SecFromMS(IN T param)
{
    return param * 1e-3;
}

template<typename T> T Chronograph<T>::SecFromUS(IN T param)
{
    return param * 1e-6;
}

template<typename T> T Chronograph<T>::SecFromNS(IN T param)
{
    return param * 1e-9;
}

} // namespace sbl